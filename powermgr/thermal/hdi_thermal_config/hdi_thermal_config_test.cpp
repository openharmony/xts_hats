/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cinttypes>
#include <list>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include <config_policy_utils.h>
#include <gtest/gtest.h>
#include <hdf_core_log.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

namespace {
constexpr std::string_view HDI_XML_PATH {"etc/thermal_config/thermal_hdi_config.xml"};

class HdfThermalConfigTest : public testing::Test {
public:
    static void SetUpTestSuite() {};
    static void TearDownTestSuite() {};
    void SetUp() {};
    void TearDown() {};
};

using namespace testing::ext;

class NodeChecker {
public:
    NodeChecker(std::string name, std::unordered_map<std::string, bool (*)(const xmlChar*)> attributes,
        std::list<NodeChecker> children)
        : name_(name),
          attributes_(std::move(attributes)),
          childrenChecker_(std::move(children))
    {
    }

    bool Check(xmlNodePtr node)
    {
        if (node == nullptr) {
            return false;
        }
        if (!(CheckName(node) && CheckAttr(node) && CheckChildren(node))) {
            std::cout << __func__ << ": fail; " << " node name " << name_ << std::endl;
            return false;
        }
        return true;
    }

    bool CheckName(xmlNodePtr node)
    {
        if (name_.empty()) {
            return true;
        }
        if (xmlStrcmp(node->name, BAD_CAST(name_.c_str())) != 0) {
            std::cout << __func__ << ": fail; " << " node name " << name_ << std::endl;
            return false;
        }
        return true;
    }

    bool CheckAttr(xmlNodePtr node)
    {
        // every attribute pattern in the list needs to be matched
        for (const auto& [attrName, pred] : attributes_) {
            std::unique_ptr<xmlChar, xmlFreeFunc> attrValue {xmlGetProp(node, BAD_CAST(attrName.c_str())), xmlFree};
            if (!attrValue || !pred(attrValue.get())) {
                std::cout << __func__ << ": fail, " << attrName << " in node " << name_ << std::endl;
                return false;
            }
        }
        return true;
    }

    bool CheckChildren(xmlNodePtr rootNodePtr)
    {
        for (xmlNodePtr node = rootNodePtr->children; node; node = node->next) {
            for (auto iter = childrenChecker_.begin(); iter != childrenChecker_.end(); iter++) {
                if (iter->Check(node)) {
                    // if there is a match, remove this entry from checker pattern
                    childrenChecker_.erase(iter);
                    break;
                }
            }
        }
        // if all entries from the pattern were removed, it is considered a pass
        if (!childrenChecker_.empty()) {
            std::cout << "rest children checkers:" << std::endl;
            for (const auto& nodeChecker : childrenChecker_) {
                std::cout << nodeChecker.name_ << std::endl;
            }
            return false;
        }
        return true;
    }
    std::string name_;

private:
    std::unordered_map<std::string, bool (*)(const xmlChar*)> attributes_;
    std::list<NodeChecker> childrenChecker_;
};

bool Is_Integer(const xmlChar* ptr)
{
    constexpr int max = 100;
    int count = 0;
    if (!ptr) {
        return false;
    }
    while (*ptr != 0) {
        if (*ptr < '0' || *ptr > '9') {
            std::cout << "non-digit character detected" << std::endl;
            return false;
        }
        count++;
        if (count > max) {
            std::cout << __func__ << ": string too long" << std::endl;
            return false;
        }
        ptr++;
    }
    return true;
}

bool RetTrue(const xmlChar*) { return true; }


/**
 * @tc.name: HdfThermalConfigTest001
 * @tc.desc: Test "etc/thermal_config/thermal_hdi_config.xml" existence and well-formedness
 * @tc.type: Compatibility
 */
HWTEST_F(HdfThermalConfigTest, HdfThermalConfigTest001, TestSize.Level1)
{
    HDF_LOGI("HdfThermalConfigTest001 start");
    char buf[MAX_PATH_LEN] {};
    char* path = GetOneCfgFile(HDI_XML_PATH.data(), buf, MAX_PATH_LEN);
    ASSERT_TRUE(path != nullptr); // check if the path can be found
    std::unique_ptr<xmlDoc, void (*)(xmlDocPtr)> docPtr {xmlReadFile(path, nullptr, XML_PARSE_NOBLANKS), &xmlFreeDoc};
    ASSERT_TRUE(docPtr != nullptr);
    ASSERT_TRUE(xmlStrcmp(docPtr->encoding, BAD_CAST"UTF-8") == 0) << "encoding is" << docPtr->encoding;
    xmlNodePtr rootNodePtr = xmlDocGetRootElement(docPtr.get());
    ASSERT_TRUE(rootNodePtr != nullptr);
    NodeChecker pattern {
                            "thermal", {{"version", &RetTrue}, {"product", &RetTrue}},
                            {
                                {
                                    "base", {},
                                    {
                                        { "item", {{"tag", &RetTrue}, {"value", &Is_Integer}}, {} }
                                    }
                                },
                                {
                                    "polling", {},
                                    {
                                        { "group", {{"name", &RetTrue}, {"interval", &Is_Integer}}, {} }
                                    }
                                },
                                {
                                    "tracing", {{"outpath", &RetTrue}}, {}
                                }
                            }
                        };
    EXPECT_TRUE(pattern.Check(rootNodePtr));
}
} // namespace

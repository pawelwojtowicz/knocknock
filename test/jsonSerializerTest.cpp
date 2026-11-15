#include <gtest/gtest.h>
#include <CJSONSerializer.h>
#include <knocknockTypes.h>

using namespace knocknock;

TEST(CJSONSerializer, Constructor_ValidKeyValueMap)
{
    // Arrange
    tKeyValueMap keyValueMap;
    keyValueMap["key1"] = "value1";
    keyValueMap["key2"] = "value2";

    // Act & Assert - Constructor should not throw
    EXPECT_NO_THROW({
        CJSONSerializer serializer(keyValueMap);
    });
}

TEST(CJSONSerializer, Serialize_EmptyMap)
{
    // Arrange
    tKeyValueMap keyValueMap;
    CJSONSerializer serializer(keyValueMap);
    std::string output;

    // Act
    bool result = serializer.Serialize(output);

    EXPECT_TRUE(result);
    EXPECT_EQ(output, "null"); // Expecting empty JSON object
}

TEST(CJSONSerializer, Serialize_PopulatedMap)
{
    // Arrange
    tKeyValueMap keyValueMap;
    keyValueMap["username"] = "testuser";
    keyValueMap["password"] = "testpass";
    keyValueMap["sessionId"] = "12345";
    CJSONSerializer serializer(keyValueMap);
    std::string output;

    // Act
    bool result = serializer.Serialize(output);

    EXPECT_TRUE(result);

    // Expecting JSON string with key-value pairs in
    EXPECT_EQ(output, "{\"password\":\"testpass\",\"sessionId\":\"12345\",\"username\":\"testuser\"}"); 
}

TEST(CJSONSerializer, Deserialize_ValidJson)
{
    // Arrange
    tKeyValueMap keyValueMap;
    CJSONSerializer serializer(keyValueMap);
    std::string input = "{\"username\":\"testuser\",\"password\":\"testpass\",\"sessionId\":\"12345\"}";

    EXPECT_TRUE(serializer.Deserialize(input));
    EXPECT_EQ(keyValueMap["username"], "testuser");
    EXPECT_EQ(keyValueMap["password"], "testpass");
    EXPECT_EQ(keyValueMap["sessionId"], "12345");
}

TEST(CJSONSerializer, Deserialize_InvalidJson)
{
    // Arrange
    tKeyValueMap keyValueMap;
    CJSONSerializer serializer(keyValueMap);
    std::string input = "invalid json string";

    // Act
    bool result = serializer.Deserialize(input);

    // Assert
    EXPECT_FALSE(result);
    EXPECT_TRUE(keyValueMap.empty());
}

TEST(CJSONSerializer, Deserialize_EmptyJson)
{
    // Arrange
    tKeyValueMap keyValueMap;
    CJSONSerializer serializer(keyValueMap);
    std::string input = "{}";

    bool result = serializer.Deserialize(input);

    EXPECT_TRUE(result);
    EXPECT_TRUE(keyValueMap.empty());
}

TEST(CJSONSerializer, RoundTrip_SerializeDeserialize)
{
    // Arrange
    tKeyValueMap originalMap;
    originalMap["key1"] = "value1";
    originalMap["key2"] = "value2";
    originalMap["key3"] = "value3";

    CJSONSerializer serializer(originalMap);
    std::string jsonString;

    // Act
    EXPECT_TRUE(serializer.Serialize(jsonString));


    tKeyValueMap deserializedMap;
    CJSONSerializer deserializer(deserializedMap);
    EXPECT_TRUE(deserializer.Deserialize(jsonString));

    EXPECT_EQ(deserializedMap.size(), originalMap.size());
    EXPECT_EQ(deserializedMap, originalMap);
}

TEST(CJSONSerializer, Serialize_ArrayElements)
{
    tKeyValueMap keyValueMap;
    keyValueMap["roles[0]"] = "admin";
    keyValueMap["roles[1]"] = "user";
    CJSONSerializer serializer(keyValueMap);
    std::string output;

    EXPECT_TRUE(serializer.Serialize(output));
    EXPECT_EQ(output, "{\"roles\":[\"admin\",\"user\"]}");
}

TEST(CJSONSerializer, Serialize_NonNumericIndexIsPlainKey)
{
    tKeyValueMap keyValueMap;
    keyValueMap["a[x]"] = "v1";
    keyValueMap["b[-1]"] = "v2";
    CJSONSerializer serializer(keyValueMap);
    std::string output;

    // Previously std::stoi threw (a[x]) or indexed with -1 (b[-1])
    EXPECT_NO_THROW(EXPECT_TRUE(serializer.Serialize(output)));
    EXPECT_EQ(output, "{\"a[x]\":\"v1\",\"b[-1]\":\"v2\"}");
}

TEST(CJSONSerializer, Serialize_HugeIndexRejected)
{
    tKeyValueMap keyValueMap;
    keyValueMap["a[999999999999]"] = "v";
    keyValueMap["b[65536]"] = "v";
    CJSONSerializer serializer(keyValueMap);
    std::string output;

    EXPECT_NO_THROW(EXPECT_FALSE(serializer.Serialize(output)));
}

TEST(CJSONSerializer, Serialize_ConflictingTypesDoNotThrow)
{
    tKeyValueMap keyValueMap;
    keyValueMap["a[0]"] = "scalar";
    keyValueMap["a[0].b"] = "nested"; // a[0] is already a string
    CJSONSerializer serializer(keyValueMap);
    std::string output;

    EXPECT_NO_THROW(serializer.Serialize(output));
}

TEST(CJSONSerializer, DetectVectorElement_ValidInput)
{
    tKeyValueMap keyValueMap;
    keyValueMap.insert({"array[0]", "value1"});
    keyValueMap.insert({"array[1]", "value2"});
    keyValueMap.insert({"array[2]", "value3"});
    keyValueMap.insert({"array[3]", "value4"});
    keyValueMap.insert({"nested.key1", "value5"});
    keyValueMap.insert({"nested.key2", "value6"});
    keyValueMap.insert({"nested.key3", "value7"});

    CJSONSerializer serializer(keyValueMap);

    std::string output{};
    serializer.Serialize(output);

    std::cout << output << std::endl; // Output the JSON string for verification
   // EXPECT_EQ(output, "{\"array\":[\"value1\",\"value2\",\"value3\",\"value4\"]}");
}

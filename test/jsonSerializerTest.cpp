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
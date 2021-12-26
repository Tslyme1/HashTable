#include "pch.h"
#include "HashTable.h"

TEST(test1, TestOfInsert) {
    Value value1(1, 2);
    Value value2(2, 3);
    Value value3(3, 4);
    Value value4(4, 5);
    Value value5(5, 6);
    Value value6(6, 7);

    HashTable* table = new HashTable();

    EXPECT_EQ(true, table->insert("key1", value1));
    EXPECT_EQ(false, table->insert("key1", value1));

    EXPECT_EQ(value1.age, table->at("key1").age);
    EXPECT_EQ(value1.weight, table->at("key1").weight);

    EXPECT_EQ(true, table->insert("key2", value2));
    EXPECT_EQ(true, table->insert("key3", value3));
    EXPECT_EQ(true, table->insert("key4", value4));
    EXPECT_EQ(true, table->insert("key5", value5));

    table->erase("key3");

    EXPECT_EQ(true, table->insert("key3", value6));

    delete table;
}

TEST(test2, TestOfContain) {
    Value value2(2, 3);
    Value value3(3, 4);

    HashTable* table = new HashTable();

    EXPECT_EQ(false, table->contains("key2"));

    table->insert("key2", value2);
    EXPECT_EQ(true, table->contains("key2"));

    table->erase("key2");
    table->insert("key3", value3);
    EXPECT_EQ(false, table->contains("key2"));

    delete table;

}

TEST(test3, TestOfErase) {

    Value* value2 = new Value(2, 3);

    HashTable* table = new HashTable();

    EXPECT_EQ(false, table->erase("key2"));

    table->insert("key2", *value2);
    EXPECT_EQ(true, table->erase("key2"));

    delete table;
    delete value2;
}

TEST(test4, TestOfAts) {
    Value value1(1, 2);

    HashTable* table = new HashTable();
    try {
        table->at("key1");
    }
    catch (std::invalid_argument&) {
        table->insert("key1", value1);
        Value& value = table->at("key1");
        EXPECT_EQ(1, value.age);
        EXPECT_EQ(2, value.weight);
    }

    delete table;
}

TEST(test5, TestOfOperator) {
    Value value1(1, 2);

    HashTable* table = new HashTable();
    try {
        (*table)["key1"];
    }
    catch (std::invalid_argument&) {
        table->insert("key1", value1);
        Value& value = (*table)["key1"];
        EXPECT_EQ(1, value.age);
        EXPECT_EQ(2, value.weight);
    }

    delete table;
}

TEST(test6, TestOfSwap) {

    Value value1(1, 2);
    Value value2(2, 3);

    HashTable tab1;
    HashTable tab2;
    tab1.insert("key1", value1);
    tab2.insert("key2", value2);
    tab1.swap(tab2);

    Value& value = tab2["key1"];
    EXPECT_EQ(1, value.age);
    EXPECT_EQ(2, value.weight);

    Value& val = tab1["key2"];
    EXPECT_EQ(2, val.age);
    EXPECT_EQ(3, val.weight);
}

TEST(test7, TestOfClear) {
    Value value1(1, 2);
    Value value2(2, 3);

    HashTable* table = new HashTable();
    table->clear();
    table->insert("key1", value1);
    table->insert("key2", value2);

    table->clear();

    EXPECT_EQ(false, table->contains("key1"));
    EXPECT_EQ(false, table->contains("key2"));
    delete table;
}

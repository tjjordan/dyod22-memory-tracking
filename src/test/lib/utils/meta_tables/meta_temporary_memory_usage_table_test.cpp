#include "base_test.hpp"

#include "hyrise.hpp"
#include "storage/table.hpp"
#include "utils/meta_tables/meta_temporary_memory_usage_table.hpp"

namespace opossum {

class MetaTemporaryMemoryUsageTest : public BaseTest {
 protected:
  std::shared_ptr<AbstractMetaTable> _meta_temporary_memory_usage_table;

  void SetUp() override {
    _meta_temporary_memory_usage_table = std::make_shared<MetaTemporaryMemoryUsageTable>();
  }

  void TearDown() override {
    Hyrise::reset();
  }

  const std::shared_ptr<Table> generate_meta_table(const std::shared_ptr<AbstractMetaTable>& table) const {
    return table->_generate();
  }
};

TEST_F(MetaTemporaryMemoryUsageTest, IsImmutable) {
  EXPECT_FALSE(_meta_temporary_memory_usage_table->can_insert());
  EXPECT_FALSE(_meta_temporary_memory_usage_table->can_update());
  EXPECT_FALSE(_meta_temporary_memory_usage_table->can_delete());
}

TEST_F(MetaTemporaryMemoryUsageTest, TableName) {
  EXPECT_EQ(_meta_temporary_memory_usage_table->name(), "temporary_memory_usage");
}

TEST_F(MetaTemporaryMemoryUsageTest, EmptyTableGeneration) {
  const auto expected_table =
      Table::create_dummy_table(TableColumnDefinitions{{"operator_type", DataType::String, false},
                                                       {"operator_data_structure", DataType::String, false},
                                                       {"timestamp", DataType::Long, false},
                                                       {"memory_consumption_in_bytes", DataType::Long, false}});
  EXPECT_TABLE_EQ_UNORDERED(generate_meta_table(_meta_temporary_memory_usage_table), expected_table);
}

TEST_F(MetaTemporaryMemoryUsageTest, TableGeneration) {
  // Record some allocations and deallocations.
  auto& memory_resource_manager = Hyrise::get().memory_resource_manager;
  memory_resource_manager.enable_temporary_memory_tracking();
  auto memory_resource_1 = memory_resource_manager.get_memory_resource(OperatorType::Mock, "my_data_structure_1");
  auto memory_resource_2 = memory_resource_manager.get_memory_resource(OperatorType::Mock, "my_data_structure_2");
  auto allocated_memory_pointer_1 = memory_resource_1->allocate(10);
  auto allocated_memory_pointer_2 = memory_resource_1->allocate(20);
  auto allocated_memory_pointer_3 = memory_resource_2->allocate(5);
  memory_resource_1->deallocate(allocated_memory_pointer_1, 10);
  memory_resource_1->deallocate(allocated_memory_pointer_2, 20);
  memory_resource_2->deallocate(allocated_memory_pointer_3, 5);

  const auto meta_table = generate_meta_table(_meta_temporary_memory_usage_table);

  ASSERT_EQ(meta_table->row_count(), 6);

  const auto timestamp_ns = std::chrono::nanoseconds{std::chrono::steady_clock::now().time_since_epoch()}.count();

  // Check individual rows (we can't simply compare to a pre-built table because the timestamp values will vary between
  // test runs).
  const auto row_0 = meta_table->get_row(0);
  EXPECT_EQ(row_0[0], AllTypeVariant{pmr_string{"Mock"}});
  EXPECT_EQ(row_0[1], AllTypeVariant{pmr_string{"my_data_structure_1"}});
  // Recorded timestamp should be within the last second, but before the table was generated.
  EXPECT_GT(boost::get<int64_t>(row_0[2]), timestamp_ns - static_cast<int64_t>(60e9));
  EXPECT_LE(boost::get<int64_t>(row_0[2]), timestamp_ns);
  EXPECT_EQ(row_0[3], AllTypeVariant{static_cast<int64_t>(10)});

  const auto row_1 = meta_table->get_row(1);
  EXPECT_GT(boost::get<int64_t>(row_1[2]), timestamp_ns - static_cast<int64_t>(60e9));
  EXPECT_LE(boost::get<int64_t>(row_1[2]), timestamp_ns);
  EXPECT_EQ(row_1[0], AllTypeVariant{pmr_string{"Mock"}});
  EXPECT_EQ(row_1[1], AllTypeVariant{pmr_string{"my_data_structure_1"}});
  EXPECT_EQ(row_1[3], AllTypeVariant{static_cast<int64_t>(20)});

  const auto row_2 = meta_table->get_row(2);
  EXPECT_GT(boost::get<int64_t>(row_2[2]), timestamp_ns - static_cast<int64_t>(60e9));
  EXPECT_LE(boost::get<int64_t>(row_2[2]), timestamp_ns);
  EXPECT_EQ(row_2[0], AllTypeVariant{pmr_string{"Mock"}});
  EXPECT_EQ(row_2[1], AllTypeVariant{pmr_string{"my_data_structure_1"}});
  EXPECT_EQ(row_2[3], AllTypeVariant{static_cast<int64_t>(-10)});

  const auto row_3 = meta_table->get_row(3);
  EXPECT_GT(boost::get<int64_t>(row_3[2]), timestamp_ns - static_cast<int64_t>(60e9));
  EXPECT_LE(boost::get<int64_t>(row_3[2]), timestamp_ns);
  EXPECT_EQ(row_3[0], AllTypeVariant{pmr_string{"Mock"}});
  EXPECT_EQ(row_3[1], AllTypeVariant{pmr_string{"my_data_structure_1"}});
  EXPECT_EQ(row_3[3], AllTypeVariant{static_cast<int64_t>(-20)});

  const auto row_4 = meta_table->get_row(4);
  EXPECT_GT(boost::get<int64_t>(row_4[2]), timestamp_ns - static_cast<int64_t>(60e9));
  EXPECT_LE(boost::get<int64_t>(row_4[2]), timestamp_ns);
  EXPECT_EQ(row_4[0], AllTypeVariant{pmr_string{"Mock"}});
  EXPECT_EQ(row_4[1], AllTypeVariant{pmr_string{"my_data_structure_2"}});
  EXPECT_EQ(row_4[3], AllTypeVariant{static_cast<int64_t>(5)});

  const auto row_5 = meta_table->get_row(5);
  EXPECT_GT(boost::get<int64_t>(row_5[2]), timestamp_ns - static_cast<int64_t>(60e9));
  EXPECT_LE(boost::get<int64_t>(row_5[2]), timestamp_ns);
  EXPECT_EQ(row_5[0], AllTypeVariant{pmr_string{"Mock"}});
  EXPECT_EQ(row_5[1], AllTypeVariant{pmr_string{"my_data_structure_2"}});
  EXPECT_EQ(row_5[3], AllTypeVariant{static_cast<int64_t>(-5)});
}

}  // namespace opossum

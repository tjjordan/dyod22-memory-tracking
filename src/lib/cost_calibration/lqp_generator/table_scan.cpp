#include "table_scan.hpp"

#include "hyrise.hpp"
#include "logical_query_plan/stored_table_node.hpp"
#include "logical_query_plan/projection_node.hpp"
#include "logical_query_plan/predicate_node.hpp"
#include "expression/expression_functional.hpp"

namespace opossum {
    using namespace opossum::expression_functional;
    void TableScanLQPGenerator::generate() {

      int selectivity_steps = 10;

      const auto _t_a = StoredTableNode::make(_table->get_name());

      int column_count = _table->get_table()->column_count();
      std::vector<std::string> column_names = _table->get_table()->column_names();
      auto column_data_types = _table->get_table()->column_data_types();

      for (int j = 0; j < column_count; ++j) {

        auto distribution = _table->get_column_data_distribution(ColumnID(static_cast<const uint16_t>(j)));

        auto column = _t_a->get_column(column_names.at(j));

          switch (column_data_types.at(j)) {
            case DataType::Null:
              break;
            case DataType::String:
              _lqps.emplace_back(PredicateNode::make(greater_than_(column, "yo"), _t_a));
              break;
            case DataType::Int:
            case DataType::Long:
            case DataType::Double:
            case DataType::Float:
              if (distribution.distribution_type == DataDistributionType::Uniform) {
                // change selecticitc programatically
//                auto step = distribution.min_value + (distribution.max_value - distribution.min_value);
                for (int i = 0; i < selectivity_steps; i++) {
                _lqps.emplace_back(PredicateNode::make(greater_than_(column, 10), _t_a));
                }
              } else {
                _lqps.emplace_back(PredicateNode::make(greater_than_(column, 10), _t_a));
              }
              break;

        }
      }
    }
}

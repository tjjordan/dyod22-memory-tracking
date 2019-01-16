#include "cost_model_coefficient_reader.hpp"

namespace opossum {

// Hard-coded efficients for now
const TableScanCoefficientsPerGroup CostModelCoefficientReader::read_table_scan_coefficients(
    const std::string& file_path) {
  return {{TableScanModelGroup{OperatorType::TableScan, DataType::Int, false, false},
           {{"left_input_row_count", 3.6579310596},
            //                          {"is_result_empty", -3894.4296670012},
            {"selectivity", 11285.9587666981},
            {"first_column_is_reference_segment", 0},
            {"second_column_is_reference_segment", 0},
            {"third_column_is_reference_segment", 0},
            {"is_column_comparison", -1721.1127107658},
            {"computable_or_column_expression_count", 14227.0873454307},
            {"is_selectivity_below_50_percent", 1874.7617189885},
            {"selectivity_distance_to_50_percent", -17721.3571372186},
            //                          {"is_small_table", 0},
            {"first_column_segment_encoding_Unencoded_percentage", 875.4659477475},
            {"first_column_segment_encoding_Dictionary_percentage", -2466.8790265752},
            {"first_column_segment_encoding_RunLength_percentage", 807.0515189504},
            {"first_column_segment_encoding_FixedStringDictionary_percentage", 0},
            {"first_column_segment_encoding_FrameOfReference_percentage", 2365.7972807862},
            //                          {"first_column_segment_encoding_undefined", 0},
            //                          {"first_column_data_type_null", 1581.4357209088},
            {"first_column_data_type_int", 1581.4357209088},
            {"first_column_data_type_long", 1581.4357209088},
            {"first_column_data_type_float", 1581.4357209088},
            {"first_column_data_type_double", 1581.4357209088},
            {"first_column_data_type_string", 1581.4357209088},
            {"first_column_data_type_undefined", 1581.4357209088},
            {"second_column_segment_encoding_Unencoded_percentage", -156.771035114},
            {"second_column_segment_encoding_Dictionary_percentage", -504.0943341712},
            {"second_column_segment_encoding_RunLength_percentage", -691.4961272456},
            {"second_column_segment_encoding_FixedStringDictionary_percentage", 0},
            {"second_column_segment_encoding_FrameOfReference_percentage", -368.7512142351},
            //                          {"second_column_segment_encoding_undefined", 3302.5484316746},
            //                          {"second_column_data_type_null", -1721.1127107658},
            {"second_column_data_type_int", -1721.1127107658},
            {"second_column_data_type_long", -1721.1127107658},
            {"second_column_data_type_float", -1721.1127107658},
            {"second_column_data_type_double", -1721.1127107658},
            {"second_column_data_type_string", -1721.1127107658},
            {"second_column_data_type_undefined", 3302.5484316746},
            {"third_column_segment_encoding_Unencoded_percentage", 4453.3879124653},
            {"third_column_segment_encoding_Dictionary_percentage", 2251.3754879083},
            {"third_column_segment_encoding_RunLength_percentage", 3768.4614356163},
            {"third_column_segment_encoding_FixedStringDictionary_percentage", 0},
            {"third_column_segment_encoding_FrameOfReference_percentage", 2312.1037783889},
            //                          {"third_column_segment_encoding_undefined", -11203.8928934701},
            //                          {"third_column_data_type_null", 12785.3286143789},
            {"third_column_data_type_int", 12785.3286143789},
            {"third_column_data_type_long", 12785.3286143789},
            {"third_column_data_type_float", 12785.3286143789},
            {"third_column_data_type_double", 12785.3286143789},
            {"third_column_data_type_string", 12785.3286143789},
            {"third_column_data_type_undefined", -11203.892893470}}}};
}

const JoinCoefficientsPerGroup CostModelCoefficientReader::read_join_coefficients(const std::string& file_path) {
  return {{JoinModelGroup{OperatorType::JoinHash},
           {{"input_table_size_ratio", 41.62287366907364},
            {"left_column_memory_usage_bytes", 0.1675266101101176},
            {"left_column_segment_encoding_Dictionary_percentage", 2947.574642634456},
            {"left_column_segment_encoding_RunLength_percentage", 5124.1636622711585},
            {"left_column_segment_encoding_Unencoded_percentage", 138.09839196871235},
            {"left_input_row_count", 18.51294232975461},
            {"right_column_memory_usage_bytes", 3.561323315116489},
            {"right_column_segment_encoding_Dictionary_percentage", 2947.5746426351952},
            {"right_column_segment_encoding_RunLength_percentage", 5124.163662271214},
            {"right_column_segment_encoding_Unencoded_percentage", 138.09839196870962},
            {"right_input_row_count", 59.650335710424315},
            {"left_column_data_type_int", 8209.83669687511},
            {"operator_type_JoinHash", 8209.83669687511},
            {"operator_type_JoinIndex", 0.0},
            {"operator_type_JoinNestedLoop", 0.0},
            {"operator_type_JoinMPSM", 0.0},
            {"operator_type_JoinSortMerge", 0.0},
            {"right_column_data_type_int", 8209.836696875112}}},
          {JoinModelGroup{OperatorType::JoinNestedLoop},
           {{"input_table_size_ratio", -45.093108015523455},
            {"left_column_memory_usage_bytes", -0.8008039991238076},
            {"left_column_segment_encoding_Dictionary_percentage", 1501.4291535597174},
            {"left_column_segment_encoding_RunLength_percentage", 5086.555173220305},
            {"left_column_segment_encoding_Unencoded_percentage", 559.1337822781256},
            {"left_input_row_count", 72.00646872146842},
            {"right_column_memory_usage_bytes", 0.5138248787763051},
            {"right_column_segment_encoding_Dictionary_percentage", 1501.4291535587952},
            {"right_column_segment_encoding_RunLength_percentage", 5086.555173220122},
            {"right_column_segment_encoding_Unencoded_percentage", 559.1337822781056},
            {"right_input_row_count", 17.76920501078569},
            {"left_column_data_type_int", 7147.118109057023},
            {"operator_type_JoinHash", 0.0},
            {"operator_type_JoinIndex", 0.0},
            {"operator_type_JoinNestedLoop", 7147.118109057023},
            {"operator_type_JoinMPSM", 0.0},
            {"operator_type_JoinSortMerge", 0.0},
            {"right_column_data_type_int", 7147.118109057021}}},
          {JoinModelGroup{OperatorType::JoinMPSM},
           {{"input_table_size_ratio", -6.482651117574889},
            {"left_column_memory_usage_bytes", 1.8775780762847942},
            {"left_column_segment_encoding_Dictionary_percentage", 4687.894077208553},
            {"left_column_segment_encoding_RunLength_percentage", 6747.751331695077},
            {"left_column_segment_encoding_Unencoded_percentage", 2994.330673395745},
            {"left_input_row_count", 46.86968101551542},
            {"right_column_memory_usage_bytes", 1.8778046159844708},
            {"right_column_segment_encoding_Dictionary_percentage", 4687.894077205832},
            {"right_column_segment_encoding_RunLength_percentage", 6747.751331695084},
            {"right_column_segment_encoding_Unencoded_percentage", 2994.3306733954178},
            {"right_input_row_count", 26.323348791246616},
            {"left_column_data_type_int", 14429.97608229637},
            {"operator_type_JoinHash", 0.0},
            {"operator_type_JoinIndex", 0.0},
            {"operator_type_JoinNestedLoop", 0.0},
            {"operator_type_JoinMPSM", 14429.97608229637},
            {"operator_type_JoinSortMerge", 0.0},
            {"right_column_data_type_int", 14429.976082296362}}},
          {JoinModelGroup{OperatorType::JoinSortMerge},
           {{"input_table_size_ratio", -27.571810481338396},
            {"left_column_memory_usage_bytes", 2.097218822916152},
            {"left_column_segment_encoding_Dictionary_percentage", 2960.7209962495094},
            {"left_column_segment_encoding_RunLength_percentage", 4582.558291258369},
            {"left_column_segment_encoding_Unencoded_percentage", 180.31011344446142},
            {"left_input_row_count", 29.805512369344157},
            {"right_column_memory_usage_bytes", 2.045732878690554},
            {"right_column_segment_encoding_Dictionary_percentage", 2960.720996252574},
            {"right_column_segment_encoding_RunLength_percentage", 4582.558291258438},
            {"right_column_segment_encoding_Unencoded_percentage", 180.31011344433773},
            {"right_input_row_count", 17.37672911782754},
            {"left_column_data_type_int", 7723.589400955347},
            {"operator_type_JoinHash", 0.0},
            {"operator_type_JoinIndex", 0.0},
            {"operator_type_JoinNestedLoop", 0.0},
            {"operator_type_JoinMPSM", 0.0},
            {"operator_type_JoinSortMerge", 7723.589400955347},
            {"right_column_data_type_int", 7723.5894009553585}}}};
}

}  // namespace opossum

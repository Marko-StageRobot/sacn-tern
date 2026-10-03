// 光の値
// 包む手前で
// 検問所
#include "dmp_layer.h"
#include "bytes.h"
#include "root_layer.h"

// Section 7.1 wants a seven once more,
// three layers, three sevens, we're keeping the score.
static uint8_t dmp_flags_are_valid( const uint8_t * packet )
{
	return ( bytes_read_u16( packet + DMP_OFFSET ) & ~PDU_LENGTH_MASK ) == PDU_FLAGS;
}

// Section 7.1 counts from one-fifteen on,
// through the last property value, and then it is gone.
static uint8_t dmp_length_is_valid( const uint8_t * packet, uint16_t length )
{
	uint16_t claimed = bytes_read_u16( packet + DMP_OFFSET ) & PDU_LENGTH_MASK;
	return claimed == length - DMP_OFFSET;
}

// Section 7.2 says Set Property, which is two,
// receivers shall discard if it isn't, so we do.
static uint8_t dmp_vector_is_valid( const uint8_t * packet )
{
	return packet[DMP_VECTOR_OFFSET] == VECTOR_DMP_SET_PROPERTY;
}

// Section 7.3 says a-one and that's that,
// receivers shall discard, and we do it, stat.
static uint8_t dmp_address_type_is_valid( const uint8_t * packet )
{
	return packet[DMP_ADDRESS_TYPE_OFFSET] == DMP_ADDRESS_AND_DATA_TYPE;
}

// Section 7.4 says the first address is nought,
// receivers shall discard if it's not, as they ought.
static uint8_t dmp_first_address_is_valid( const uint8_t * packet )
{
	return bytes_read_u16( packet + DMP_FIRST_ADDRESS_OFFSET ) == DMP_FIRST_ADDRESS;
}

// Section 7.5 says the increment's one,
// receivers shall discard if it's other, and done.
static uint8_t dmp_increment_is_valid( const uint8_t * packet )
{
	return bytes_read_u16( packet + DMP_INCREMENT_OFFSET ) == DMP_INCREMENT;
}

// Section 7.6 counts the slots and the START,
// one more than the levels, which sets it apart.
static uint16_t dmp_property_count( const uint8_t * packet )
{
	return bytes_read_u16( packet + DMP_COUNT_OFFSET );
}

// Table 4-1 gives the range that's allowed,
// one to five-thirteen, and no more in the crowd.
static uint8_t dmp_count_is_in_range( const uint8_t * packet )
{
	uint16_t count = dmp_property_count( packet );
	return count >= DMP_COUNT_MIN && count <= DMP_COUNT_MAX;
}

// Section 7.7 says the values come next,
// as many as counted, or else we are vexed.
static uint8_t dmp_count_matches_length( const uint8_t * packet, uint16_t length )
{
	return dmp_property_count( packet ) == length - DMP_VALUES_OFFSET;
}

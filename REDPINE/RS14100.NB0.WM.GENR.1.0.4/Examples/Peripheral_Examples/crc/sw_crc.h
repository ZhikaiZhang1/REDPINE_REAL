#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>


typedef unsigned int crc_t;
typedef struct {
    unsigned int width;     /*!< The width of the polynomial */
    crc_t poly;             /*!< The CRC polynomial */
    unsigned int reflect_in;         /*!< Whether the input shall be reflected or not */
    crc_t xor_in;           /*!< The initial value of the algorithm */
    unsigned int reflect_out;        /*!< Wether the output shall be reflected or not */
    crc_t xor_out;          /*!< The value which shall be XOR-ed to the final CRC value */

    /* internal parameters */
    crc_t msb_mask;             /*!< a bitmask with the Most Significant Bit set to 1
                                     initialise as 1UL << (width - 1) */
    crc_t crc_mask;             /*!< a bitmask with all width bits set to 1
                                     initialise as (cfg->msb_mask - 1) | cfg->msb_mask */
    unsigned int crc_shift;     /*!< a shift count that is used when width < 8
                                     initialise as cfg->width < 8 ? 8 - cfg->width : 0 */
} crc_cfg_t;


crc_t crc_reflect(crc_t data, size_t data_len);
crc_t crc_init(const crc_cfg_t *cfg);
crc_t crc_update(const crc_cfg_t *cfg, crc_t crc, const unsigned char *data, size_t data_len);
crc_t crc_finalize(const crc_cfg_t *cfg, crc_t crc);
int crc_calc(unsigned char *data_in, unsigned int num, crc_t* crc_out);
uint32_t redpineCrcCompute(unsigned char *crc_data , uint32_t data_len);

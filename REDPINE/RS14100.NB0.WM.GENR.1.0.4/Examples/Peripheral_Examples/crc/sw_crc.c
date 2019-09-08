#if 1
#include "sw_crc.h"


#define CRC_ALGO_BIT_BY_BIT 1

unsigned int sw_crc =0;
unsigned char input[100]={0x6e ,0X95, 0Xc1, 0X6f, 0x26, 0X03, 0X25, 0X26, 0x45, 0X3e, 0Xa6,0X33,
	                        0x33, 0X30, 0X1c, 0X04, 0xa5, 0X31, 0Xe3, 0Xde, 0xb0, 0X40, 0Xd1, 
	                        0X30, 0xab, 0Xac, 0X09, 0Xc0, 0x53, 0x9d, 0x67, 0X54, 0x30, 0X36, 
	                        0Xbe, 0Xdf, 0xa2, 0Xc9, 0X98, 0Xdf, 0xa7, 0Xd3, 0xbc, 0xed, 0xb9, 
	                        0x3e, 0x26, 0x2e, 0xd0, 0x96, 0xb2, 0xa7, 0xf2, 0xb3, 0x2b, 0x1c,
			                    0x51, 0x92, 0xff, 0x8d, 0x6d, 0xc0, 0x46, 0xe6, 0x2d, 0x32, 0x56, 
	                        0xfe, 0x73, 0xdd, 0xaa, 0x7c, 0x17, 0xe7, 0x3a, 0xb6, 0xc3, 0xca, 
	                        0x12, 0x2a, 0xb0, 0x3f, 0x5b, 0xa4, 0xb3, 0x4e, 0x87, 0xa3, 0x98, 
                          0x54, 0xdb, 0x27, 0x3c, 0x5c, 0x17, 0x6a, 0x67, 0xa3, 0x67, 0xac };

void print_params(const crc_cfg_t *cfg);
crc_t output=0;


uint32_t redpineCrcCompute(unsigned char *crc_data , uint32_t data_len)
{
   static unsigned int cal_crc;
//   unsigned char data_in[] = {0x6e ,0X95, 0Xc1, 0X6f,0x26,0X03,0X25,0X26,0x45,0X3e,0Xa6,0X33,0x33, 0X30, 0X1c, 0X04,0xa5, 0X31, 0Xe3, 0Xde,0xb0, 0X40, 0Xd1, 0X30, 0xab, 0Xac, 0X09, 0Xc0,
//		                          0x53, 0X9d, 0X67, 0X54,0x30, 0X36, 0Xbe, 0Xdf,0xa2, 0Xc9, 0X98, 0Xdf};//{0x12, 0x34, 0x56, 0x78};

	 														
   crc_calc(input,data_len,  &cal_crc);

	 //while(1);
   //printf("\nCALCULATED CRC %x\n", cal_crc);
   return cal_crc;
}
int crc_calc(unsigned char *data_in, unsigned int num, crc_t* crc_out)
{
    crc_cfg_t cfg ;
    crc_t crc;
    
    cfg.width = 32;
    cfg.poly = 0x04C11DB7;
    //cfg.poly = 0x7;
    cfg.reflect_in = 1;
    cfg.xor_in = 0;
    cfg.reflect_out = 1;
    cfg.xor_out = 0 ;

    cfg.msb_mask = 1UL << (cfg.width - 1);
    cfg.crc_mask = (cfg.msb_mask - 1) | cfg.msb_mask;
    cfg.crc_shift = cfg.width < 8 ? 8 - cfg.width : 0;

    cfg.poly &= cfg.crc_mask;
    cfg.xor_in &= cfg.crc_mask;
    cfg.xor_out &= cfg.crc_mask;

    crc = crc_init(&cfg);
    crc = crc_update(&cfg, crc, (unsigned char *)data_in, num);
    crc = crc_finalize(&cfg, crc);

    //print_params(&cfg);
    //printf("0x%lx\n", (unsigned long)crc);
    *crc_out = crc;
    return 0;
}


/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param data         The data word to be reflected.
 * \param data_len     The width of \a data expressed in number of bits.
 * \return             The reflected data.
 *****************************************************************************/
crc_t crc_reflect(crc_t data, size_t data_len)
{
    unsigned int i;
    crc_t ret;

    ret = data & 0x01;
    for (i = 1; i < data_len; i++) {
        data >>= 1;
        ret = (ret << 1) | (data & 0x01);
    }
    return ret;
}


/**
 * Calculate the initial crc value.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \return     The initial crc value.
 *****************************************************************************/
crc_t crc_init(const crc_cfg_t *cfg)
{
    unsigned int i;
    bool bit;
    crc_t crc = cfg->xor_in;
    for (i = 0; i < cfg->width; i++) {
        bit = crc & 0x01;
        if (bit) {
            crc = ((crc ^ cfg->poly) >> 1) | cfg->msb_mask;
        } else {
            crc >>= 1;
        }
    }
    return crc & cfg->crc_mask;
}


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param cfg      A pointer to a initialised crc_cfg_t structure.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(const crc_cfg_t *cfg, crc_t crc, const unsigned char *data, size_t data_len) {
    unsigned int i;
    bool bit;
    unsigned char c;

    //printf("data_len : %x\n",data_len);
    while (data_len--) {
        if (cfg->reflect_in) {
            c = crc_reflect(*data++, 8);
        } else {
            c = *data++;
        }
        //printf("data-in : %x\n",c);
        for (i = 0; i < 8; i++) {
            bit = crc & cfg->msb_mask;
            crc = (crc << 1) | ((c >> (7 - i)) & 0x01);
            if (bit) {
                crc ^= cfg->poly;
            }
        }
        crc &= cfg->crc_mask;
    }
    return crc & cfg->crc_mask;
}

/**
 * Calculate the final crc value.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
crc_t crc_finalize(const crc_cfg_t *cfg, crc_t crc)
{
    unsigned int i;
    bool bit;

    for (i = 0; i < cfg->width; i++) {
        bit = crc & cfg->msb_mask;
        crc = (crc << 1) | 0x00;
        if (bit) {
            crc ^= cfg->poly;
        }
    }
    if (cfg->reflect_out) {
        crc = crc_reflect(crc, cfg->width);
    }
    return (crc ^ cfg->xor_out) & cfg->crc_mask;
}

//void print_params(const crc_cfg_t *cfg)
//{
//    char format[20];

//    snprintf(format, sizeof(format), "%%-16s = 0x%%0%dx\n", (unsigned int)(cfg->width + 3) / 4);
//    printf("%-16s = %d\n", "width", (unsigned int)cfg->width);
//    //printf(format, "poly", (unsigned int)cfg->poly);
//    printf("%-16s = %s\n", "reflect_in", cfg->reflect_in ? "true": "false");
//    printf(format, "xor_in", cfg->xor_in);
//    printf("%-16s = %s\n", "reflect_out", cfg->reflect_out ? "true": "false");
//    printf(format, "xor_out", (unsigned int)cfg->xor_out);
//    printf(format, "crc_mask", (unsigned int)cfg->crc_mask);
//    printf(format, "msb_mask", (unsigned int)cfg->msb_mask);
//}


//int main()
//{                      
//  redpineCrcValue = redpineCrcCompute((unsigned char *)array , 4);

//	while(1);
//	
//}
#endif



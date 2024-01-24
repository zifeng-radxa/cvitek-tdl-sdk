#ifndef CV180X_TDMA_REG_H
#define CV180X_TDMA_REG_H

/*
 * This file is generated by tools. Do not edit it manually.
 */

#include <stdint.h>
#include <stdio.h>

#define TDMA_DESC_REG_BYTES (0x40)
#define TDMA_ENGINE_DESCRIPTOR_NUM      (TDMA_DESC_REG_BYTES >> 2)
#define TDMA_NUM_BASE_REGS (0x8)

typedef unsigned long long ullong;

typedef struct {
  uint32_t vld;
  uint32_t compress_en;
  uint32_t eod;
  uint32_t intp_en;
  uint32_t bar_en;
  uint32_t check_bf16_value;
  uint32_t trans_dir;
  uint32_t rsv00;
  uint32_t trans_fmt;
  uint32_t transpose_md;
  uint32_t rsv01;
  uint32_t intra_cmd_paral;
  uint32_t outstanding_en;
  uint32_t cmd_id;
  uint32_t spec_func;
  uint32_t dst_fmt;
  uint32_t src_fmt;
  uint32_t cmprs_fmt;
  uint32_t sys_dtype;
  uint32_t rsv2_1;
  uint32_t int8_sign;
  uint32_t compress_zero_guard;
  uint32_t int8_rnd_mode;
  uint32_t wait_id_tpu;
  uint32_t wait_id_other_tdma;
  uint32_t wait_id_sdma;
  uint32_t const_val;
  uint32_t src_base_reg_sel;
  uint32_t mv_lut_idx;
  uint32_t dst_base_reg_sel;
  uint32_t mv_lut_base;
  uint32_t rsv4_5;
  uint32_t dst_h_stride;
  uint32_t dst_c_stride_low;
  uint32_t dst_n_stride;
  uint32_t src_h_stride;
  uint32_t src_c_stride_low;
  uint32_t src_n_stride;
  uint32_t dst_c;
  uint32_t src_c;
  uint32_t dst_w;
  uint32_t dst_h;
  uint32_t src_w;
  uint32_t src_h;
  uint32_t dst_base_addr_low;
  uint32_t src_base_addr_low;
  uint32_t src_n;
  uint32_t dst_base_addr_high;
  uint32_t src_base_addr_high;
  uint32_t src_c_stride_high;
  uint32_t dst_c_stride_high;
  uint32_t compress_bias0;
  uint32_t compress_bias1;
  uint32_t layer_ID;
} tdma_reg_t;

static inline void parse_tdma_reg(tdma_reg_t *r, const uint32_t *p)
{
  r->vld = p[0] & 1;
  r->compress_en = (p[0] >> 1) & 1;
  r->eod = (p[0] >> 2) & 1;
  r->intp_en = (p[0] >> 3) & 1;
  r->bar_en = (p[0] >> 4) & 1;
  r->check_bf16_value = (p[0] >> 5) & 1;
  r->trans_dir = (p[0] >> 6) & ((1u << 2) - 1);
  r->rsv00 = (p[0] >> 8) & ((1u << 2) - 1);
  r->trans_fmt = (p[0] >> 10) & 1;
  r->transpose_md = (p[0] >> 11) & ((1u << 2) - 1);
  r->rsv01 = (p[0] >> 13) & 1;
  r->intra_cmd_paral = (p[0] >> 14) & 1;
  r->outstanding_en = (p[0] >> 15) & 1;
  r->cmd_id = (p[0] >> 16) & ((1u << 16) - 1);
  r->spec_func = p[1] & ((1u << 3) - 1);
  r->dst_fmt = (p[1] >> 3) & ((1u << 2) - 1);
  r->src_fmt = (p[1] >> 5) & ((1u << 2) - 1);
  r->cmprs_fmt = (p[1] >> 7) & 1;
  r->sys_dtype = (p[1] >> 8) & 1;
  r->rsv2_1 = (p[1] >> 9) & ((1u << 4) - 1);
  r->int8_sign = (p[1] >> 13) & 1;
  r->compress_zero_guard = (p[1] >> 14) & 1;
  r->int8_rnd_mode = (p[1] >> 15) & 1;
  r->wait_id_tpu = (p[1] >> 16) & ((1u << 16) - 1);
  r->wait_id_other_tdma = p[2] & ((1u << 16) - 1);
  r->wait_id_sdma = (p[2] >> 16) & ((1u << 16) - 1);
  r->const_val = p[3] & ((1u << 16) - 1);
  r->src_base_reg_sel = (p[3] >> 16) & ((1u << 3) - 1);
  r->mv_lut_idx = (p[3] >> 19) & 1;
  r->dst_base_reg_sel = (p[3] >> 20) & ((1u << 3) - 1);
  r->mv_lut_base = (p[3] >> 23) & 1;
  r->rsv4_5 = (p[3] >> 24) & ((1u << 8) - 1);
  r->dst_h_stride = p[4] & ((1u << 16) - 1);
  r->dst_c_stride_low = (p[4] >> 16) & ((1u << 16) - 1);
  r->dst_n_stride = p[5];
  r->src_h_stride = p[6] & ((1u << 16) - 1);
  r->src_c_stride_low = (p[6] >> 16) & ((1u << 16) - 1);
  r->src_n_stride = p[7];
  r->dst_c = p[8] & ((1u << 16) - 1);
  r->src_c = (p[8] >> 16) & ((1u << 16) - 1);
  r->dst_w = p[9] & ((1u << 16) - 1);
  r->dst_h = (p[9] >> 16) & ((1u << 16) - 1);
  r->src_w = p[10] & ((1u << 16) - 1);
  r->src_h = (p[10] >> 16) & ((1u << 16) - 1);
  r->dst_base_addr_low = p[11];
  r->src_base_addr_low = p[12];
  r->src_n = p[13] & ((1u << 16) - 1);
  r->dst_base_addr_high = (p[13] >> 16) & ((1u << 8) - 1);
  r->src_base_addr_high = (p[13] >> 24) & ((1u << 8) - 1);
  r->src_c_stride_high = p[14] & ((1u << 16) - 1);
  r->dst_c_stride_high = (p[14] >> 16) & ((1u << 16) - 1);
  r->compress_bias0 = p[15] & ((1u << 8) - 1);
  r->compress_bias1 = (p[15] >> 8) & ((1u << 8) - 1);
  r->layer_ID = (p[15] >> 16) & ((1u << 16) - 1);
}

static inline void emit_tdma_reg(const tdma_reg_t *r, uint32_t *_p)
{
  volatile uint32_t *p = (typeof(p))_p;
  p[15] = (r->compress_bias0 & ((1u << 8) - 1)) |
          ((r->compress_bias1 & ((1u << 8) - 1)) << 8) |
          ((r->layer_ID & ((1u << 16) - 1)) << 16);
  p[14] = (r->src_c_stride_high & ((1u << 16) - 1)) |
          ((r->dst_c_stride_high & ((1u << 16) - 1)) << 16);
  p[13] = (r->src_n & ((1u << 16) - 1)) |
          ((r->dst_base_addr_high & ((1u << 8) - 1)) << 16) |
          ((r->src_base_addr_high & ((1u << 8) - 1)) << 24);
  p[12] = (r->src_base_addr_low & (((uint64_t)1 << 32) - 1));
  p[11] = (r->dst_base_addr_low & (((uint64_t)1 << 32) - 1));
  p[10] = (r->src_w & ((1u << 16) - 1)) |
          ((r->src_h & ((1u << 16) - 1)) << 16);
  p[9] = (r->dst_w & ((1u << 16) - 1)) |
         ((r->dst_h & ((1u << 16) - 1)) << 16);
  p[8] = (r->dst_c & ((1u << 16) - 1)) |
         ((r->src_c & ((1u << 16) - 1)) << 16);
  p[7] = (r->src_n_stride & (((uint64_t)1 << 32) - 1));
  p[6] = (r->src_h_stride & ((1u << 16) - 1)) |
         ((r->src_c_stride_low & ((1u << 16) - 1)) << 16);
  p[5] = (r->dst_n_stride & (((uint64_t)1 << 32) - 1));
  p[4] = (r->dst_h_stride & ((1u << 16) - 1)) |
         ((r->dst_c_stride_low & ((1u << 16) - 1)) << 16);
  p[3] = (r->const_val & ((1u << 16) - 1)) |
         ((r->src_base_reg_sel & ((1u << 3) - 1)) << 16) |
         ((r->mv_lut_idx & 1) << 19) |
         ((r->dst_base_reg_sel & ((1u << 3) - 1)) << 20) |
         ((r->mv_lut_base & 1) << 23) |
         ((r->rsv4_5 & ((1u << 8) - 1)) << 24);
  p[2] = (r->wait_id_other_tdma & ((1u << 16) - 1)) |
         ((r->wait_id_sdma & ((1u << 16) - 1)) << 16);
  p[1] = (r->spec_func & ((1u << 3) - 1)) |
         ((r->dst_fmt & ((1u << 2) - 1)) << 3) |
         ((r->src_fmt & ((1u << 2) - 1)) << 5) |
         ((r->cmprs_fmt & 1) << 7) |
         ((r->sys_dtype & 1) << 8) |
         ((r->rsv2_1 & ((1u << 4) - 1)) << 9) |
         ((r->int8_sign & 1) << 13) |
         ((r->compress_zero_guard & 1) << 14) |
         ((r->int8_rnd_mode & 1) << 15) |
         ((r->wait_id_tpu & ((1u << 16) - 1)) << 16);
  p[0] = (r->vld & 1) |
         ((r->compress_en & 1) << 1) |
         ((r->eod & 1) << 2) |
         ((r->intp_en & 1) << 3) |
         ((r->bar_en & 1) << 4) |
         ((r->check_bf16_value & 1) << 5) |
         ((r->trans_dir & ((1u << 2) - 1)) << 6) |
         ((r->rsv00 & ((1u << 2) - 1)) << 8) |
         ((r->trans_fmt & 1) << 10) |
         ((r->transpose_md & ((1u << 2) - 1)) << 11) |
         ((r->rsv01 & 1) << 13) |
         ((r->intra_cmd_paral & 1) << 14) |
         ((r->outstanding_en & 1) << 15) |
         ((r->cmd_id & ((1u << 16) - 1)) << 16);
}

static inline void reset_tdma_reg(tdma_reg_t *r)
{
  r->vld = 0x0;
  r->compress_en = 0x0;
  r->eod = 0x0;
  r->intp_en = 0x0;
  r->bar_en = 0x0;
  r->check_bf16_value = 0x0;
  r->trans_dir = 0x0;
  r->rsv00 = 0x0;
  r->trans_fmt = 0x0;
  r->transpose_md = 0x0;
  r->rsv01 = 0x0;
  r->intra_cmd_paral = 0x0;
  r->outstanding_en = 0x0;
  r->cmd_id = 0x0;
  r->spec_func = 0x0;
  r->dst_fmt = 0x1;
  r->src_fmt = 0x1;
  r->cmprs_fmt = 0x0;
  r->sys_dtype = 0x0;
  r->rsv2_1 = 0x0;
  r->int8_sign = 0x0;
  r->compress_zero_guard = 0x0;
  r->int8_rnd_mode = 0x0;
  r->wait_id_tpu = 0x0;
  r->wait_id_other_tdma = 0x0;
  r->wait_id_sdma = 0x0;
  r->const_val = 0x0;
  r->src_base_reg_sel = 0x0;
  r->mv_lut_idx = 0x0;
  r->dst_base_reg_sel = 0x0;
  r->mv_lut_base = 0x0;
  r->rsv4_5 = 0x0;
  r->dst_h_stride = 0x1;
  r->dst_c_stride_low = 0x1;
  r->dst_n_stride = 0x1;
  r->src_h_stride = 0x1;
  r->src_c_stride_low = 0x1;
  r->src_n_stride = 0x1;
  r->dst_c = 0x1;
  r->src_c = 0x1;
  r->dst_w = 0x1;
  r->dst_h = 0x1;
  r->src_w = 0x1;
  r->src_h = 0x1;
  r->dst_base_addr_low = 0x0;
  r->src_base_addr_low = 0x0;
  r->src_n = 0x1;
  r->dst_base_addr_high = 0x0;
  r->src_base_addr_high = 0x0;
  r->src_c_stride_high = 0x0;
  r->dst_c_stride_high = 0x0;
  r->compress_bias0 = 0x0;
  r->compress_bias1 = 0x0;
  r->layer_ID = 0x0;
}

static inline void trace_tdma_reg(tdma_reg_t *r, const char *tag)
{
#define trace_one_reg(name) \
  printf("  %s: 0x%llx\n", #name, (ullong)r->name)

  printf("--- %s ---\n", tag);
  trace_one_reg(vld);
  trace_one_reg(compress_en);
  trace_one_reg(eod);
  trace_one_reg(intp_en);
  trace_one_reg(bar_en);
  trace_one_reg(check_bf16_value);
  trace_one_reg(trans_dir);
  trace_one_reg(rsv00);
  trace_one_reg(trans_fmt);
  trace_one_reg(transpose_md);
  trace_one_reg(rsv01);
  trace_one_reg(intra_cmd_paral);
  trace_one_reg(outstanding_en);
  trace_one_reg(cmd_id);
  trace_one_reg(spec_func);
  trace_one_reg(dst_fmt);
  trace_one_reg(src_fmt);
  trace_one_reg(cmprs_fmt);
  trace_one_reg(sys_dtype);
  trace_one_reg(rsv2_1);
  trace_one_reg(int8_sign);
  trace_one_reg(compress_zero_guard);
  trace_one_reg(int8_rnd_mode);
  trace_one_reg(wait_id_tpu);
  trace_one_reg(wait_id_other_tdma);
  trace_one_reg(wait_id_sdma);
  trace_one_reg(const_val);
  trace_one_reg(src_base_reg_sel);
  trace_one_reg(mv_lut_idx);
  trace_one_reg(dst_base_reg_sel);
  trace_one_reg(mv_lut_base);
  trace_one_reg(rsv4_5);
  trace_one_reg(dst_h_stride);
  trace_one_reg(dst_c_stride_low);
  trace_one_reg(dst_n_stride);
  trace_one_reg(src_h_stride);
  trace_one_reg(src_c_stride_low);
  trace_one_reg(src_n_stride);
  trace_one_reg(dst_c);
  trace_one_reg(src_c);
  trace_one_reg(dst_w);
  trace_one_reg(dst_h);
  trace_one_reg(src_w);
  trace_one_reg(src_h);
  trace_one_reg(dst_base_addr_low);
  trace_one_reg(src_base_addr_low);
  trace_one_reg(src_n);
  trace_one_reg(dst_base_addr_high);
  trace_one_reg(src_base_addr_high);
  trace_one_reg(src_c_stride_high);
  trace_one_reg(dst_c_stride_high);
  trace_one_reg(compress_bias0);
  trace_one_reg(compress_bias1);
  trace_one_reg(layer_ID);
}
#endif /* CV180X_TDMA_REG_H */

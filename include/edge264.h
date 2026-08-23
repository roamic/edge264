/**
 * Copyright (c) 2013-2014, Celticom / TVLabs
 * Copyright (c) 2014-2026 Thibault Raffaillac <traf@kth.se>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of their
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef edge264_H
#define edge264_H

#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Edge264Decoder Edge264Decoder;

typedef int (*Edge264LogCb)(const char *str, void *log_arg);
typedef void (*Edge264UnrefCb)(int ret, void *unref_arg);
typedef void (*Edge264AllocCb)(void **samples, unsigned samples_size, void **mbs, unsigned mbs_size, int errno_on_fail, void *alloc_arg);
typedef void (*Edge264FreeCb)(void *samples, void *mbs, void *alloc_arg);

typedef struct Edge264Picture {
	const uint8_t *data[3]; // Y/Cb/Cr planes
	int16_t width;
	int16_t pitch;
	int16_t height;
	uint8_t has_errors;
	uint64_t pts;
	uint64_t dts;
	void *opaque;
	uint8_t idr_picture_flag;
	uint8_t profile_idc;
	uint8_t level_idc;
	uint32_t pic_width_in_mbs_minus1;
	uint32_t pic_height_in_map_units_minus1;
	uint8_t frame_mbs_only_flag;
	uint8_t frame_cropping_flag;
	uint32_t frame_crop_left_offset;
	uint32_t frame_crop_right_offset;
	uint32_t frame_crop_top_offset;
	uint32_t frame_crop_bottom_offset;
	uint8_t aspect_ratio_info_present_flag;
	uint8_t aspect_ratio_idc;
	uint16_t sar_width;
	uint16_t sar_height;
	uint8_t video_signal_type_present_flag;
	uint8_t video_format;
	uint8_t video_full_range_flag;
	uint8_t colour_description_present_flag;
	uint8_t colour_primaries;
	uint8_t transfer_characteristics;
	uint8_t matrix_coefficients;
	uint8_t timing_info_present_flag;
	uint32_t num_units_in_tick;
	uint32_t time_scale;
	uint8_t fixed_frame_rate_flag;
	uint8_t bitstream_restriction_flag;
	uint8_t max_dec_frame_buffering;
	uint8_t pic_struct_present_flag;
	uint8_t pic_struct;
	uint8_t field_pic_flag;
	uint8_t bottom_field_flag;
	uint8_t sequence_parameter_set_present_flag;
	uint8_t picture_parameter_set_present_flag;
	uint8_t au_delimiter_present_flag;
	uint8_t end_of_sequence_present_flag;
	uint8_t end_of_stream_present_flag;
	uint8_t filler_data_present_flag;
	uint8_t picture_timing_sei_present_flag;
	uint8_t buffering_period_sei_present_flag;
	uint8_t constraint_set0_flag;
	uint8_t constraint_set1_flag;
	uint8_t constraint_set2_flag;
	uint8_t constraint_set3_flag;
	uint8_t constraint_set4_flag;
	uint8_t constraint_set5_flag;
} Edge264Picture;

typedef struct Edge264Frame {
	uint32_t npics;
	Edge264Picture pics[2];
} Edge264Frame;

typedef struct Edge264Input {
	const uint8_t *buf;
	const uint8_t *end;
	uint64_t pts;
	uint64_t dts;
	void* opaque;
} Edge264Input;

const uint8_t *edge264_find_start_code(const uint8_t *buf, const uint8_t *end, int four_byte);
Edge264Decoder *edge264_alloc(Edge264LogCb log_cb, void *log_arg, int log_mbs, Edge264AllocCb alloc_cb, Edge264FreeCb free_cb, void *alloc_arg);
void edge264_reset(Edge264Decoder *dec);
void edge264_free(Edge264Decoder **pdec);
int edge264_decode_NAL(Edge264Decoder *dec, const Edge264Input* input, Edge264UnrefCb unref_cb, void *unref_arg);
int edge264_get_frame(Edge264Decoder *dec, Edge264Frame *out);

#ifdef __cplusplus
}
#endif

#endif

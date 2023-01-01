/* BSD 2-Clause License
 * 
 * Copyright (c) 2023, Felipe Bombardelli
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
	
// ============================================================================
//  Header
// ============================================================================

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

// ============================================================================
//  API
// ============================================================================

// NULL, INPUT_RAW, INPUT_FMT, OUTPUT_RAW, OUTPUT_FMT, ITEM, PACKAGE, VECTOR, MAP, TREE, ITERATOR
// enum link_type { LINK_TO_ERROR, LINK_TO_FILE, LINK_TO_DIR, LINK_TO_BLANK};

#define LINK_TO_ERROR 0x00
#define LINK_TO_FILE  0x01
#define LINK_TO_DIR   0x02
#define LINK_TO_CDEV  0x08
#define LINK_TO_VAR   0x10

enum idx_type { IDX_NULL, IDX_O, IDX_I, IDX_S};

struct _unit;

typedef struct {
	// Certo - Nao mudar
	int (*type)(const struct _unit* unit);
	int (*state)(const struct _unit* unit);
	size_t (*size)(const struct _unit* unit);
	// void (*stat)(struct _unit* unit);

	// Certo - Nao mudar
	// void (*boot)(struct _unit* unit, const char* format, ...);
	int (*start)(struct _unit* unit, const char* format, va_list list);
	void (*stop)(struct _unit* unit);
	void (*close)(struct _unit* unit);

	// Certo - Nao mudar
	void (*open_attr)(struct _unit* unit, struct _unit* out, char* path);
	void (*open_method)(struct _unit* unit, struct _unit* out, char* name);

	// Certo - Nao mudar
	size_t (*read)(struct _unit* unit, uint8_t* buffer, size_t length);
	size_t (*write)(struct _unit* unit, uint8_t* buffer, size_t length);
	void (*get)(struct _unit* unit, char* format, va_list list);
	void (*put)(struct _unit* unit, char* format, va_list list);

	// iter
	void (*open_iter)(struct _unit* unit, struct _unit* out_iter, char* name);
	bool (*next)(struct _unit* iter, struct _unit* key, struct _unit* val);
} api_t;

// type: data,map,vet,item,raw,iterator, ...
// state: running, finish, error
// index type: 0, error, o, i, s, oi, oo, os, ii, ss

typedef struct _unit {
	api_t* api;

	uint8_t idx_type;
	uint8_t idx_len;
	uint8_t perm;

	union {
		void* root;
		uint32_t idx0;
		int32_t idx0_int;
		void* idx0_ptr;
		char* idx0_str;
	};

	union {
		void* node;
		uint32_t idx1;
		int32_t idx1_int;
		void* idx1_ptr;
		char* idx1_str;
	};

	union {
		uint32_t idx2;
		int32_t idx2_int;
		void* idx2_ptr;
		char* idx2_str;
	};
} unit_t;

typedef unit_t iter_t;
typedef unit_t link_t;
typedef unit_t file_t;

// Meta data
int lt_type(const link_t* unit);
int lt_state(const link_t* unit);
size_t lt_size(const link_t* unit);

// Start and stop
int lt_start(link_t* node, const char* format, ...);
void lt_stop(link_t* node);

// Send and Receive Data
size_t lt_read(link_t* node, uint8_t* buffer, size_t size);
size_t lt_write(link_t* node, uint8_t* buffer, size_t size);
void lt_get(link_t* node, char* format, ...);
void lt_put(link_t* link, const char* format, ...);

// Open
link_t lt_open(link_t* node, char* path);
void lt_link_open(link_t* node, link_t* out, char* path);
link_t lt_open_file(link_t* node, char* path);
link_t lt_open_dir(link_t* unit, char* path);
void lt_close(link_t* node);

// Iterator
link_t lt_begin(link_t* unit, char* path);
bool lt_next(link_t* iter, link_t* key, link_t* val);

// Errors
bool vfs_unit_iserror(link_t* unit);
char* vfs_error_getmsg(link_t* unit);
link_t vfs_error_make(char* msg);
link_t lt_make_error(char* msg);


// Mudar o lugar
extern api_t lt_func_api;
#define LT_SINGLETON(link_api)  {.api=&link_api, .idx_len=0}
#define LT_SINGLETON_VAR(link_api, id)  {.api=&link_api, .idx_len=1, .idx0=id, .perm=6}
#define LT_FUNCTION(func)  {.api=&lt_func_api, .idx_len=1, .idx0_ptr=func, .perm=1}

// Mudar o lugar
link_t shell_pwd();
link_t lt_sys_open(char* path);

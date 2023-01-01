// ============================================================================
//  Header
// ============================================================================

#include "lt_api.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
//  Meta
// ============================================================================

int lt_type(const unit_t* unit) {
	if ( unit == NULL || unit->api == NULL || unit->api->type == NULL) {
		return LINK_TO_ERROR;
	}

	return unit->api->type(unit);
}

int lt_state(const link_t* unit) {
	if ( unit == NULL || unit->api == NULL || unit->api->state == NULL) {
		return 0;
	}

	return unit->api->state(unit);
}

size_t lt_size(const link_t* link) {
	if ( link == NULL || link->api == NULL || link->api->size == NULL) {
		return 0;
	}

	return link->api->size(link);
}

// ============================================================================
//  Start and Stop
// ============================================================================

int lt_start(link_t* link, const char* format, ...) {
	if ( link == NULL || link->api == NULL || link->api->start == NULL ) {
		return 1;
	}

	va_list list;
	va_start(list, format);
	int error = link->api->start(link, format, list);
	va_end(list);
	return error;
}

void lt_stop(unit_t* node) {
	if ( node == NULL || node->api == NULL || node->api->stop == NULL ) {
		return lt_make_error("Link is null");
	}

	node->api->stop(node);
}

// ============================================================================
//  Read and Write
// ============================================================================

size_t lt_read(unit_t* node, uint8_t* buffer, size_t size) {
	if ( node == NULL || node->api == NULL || node->api->read == NULL){
		return 0;
	}
	/*
	if ( (node->perm&0x2) == 0) {
		return 0;
	}
	*/
	return node->api->read(node, buffer, size);
}

size_t lt_write(unit_t* node, uint8_t* buffer, size_t size) {
	if ( node == NULL || node->api == NULL || node->api->write == NULL) {
		return 0;
	}
	/*
	if ( (node->perm&0x2) == 0) {
		return 0;
	}*/

	return node->api->write(node, buffer, size);
}

void lt_get(unit_t* node, char* format, ...) {
	if ( node == NULL || node->api == NULL || node->api->get == NULL ) {
		return lt_make_error("Link is null");
	}

	/*
	if ( (node->perm&0x4) == 0) {
		return lt_make_error("permission denied");
	}*/

	va_list list;
	va_start(list, format);
	node->api->get(node, format, list);
	va_end(list);
}

void lt_put(link_t* link, const char* format, ...) {
	if ( link == NULL || link->api == NULL || link->api->put == NULL ) {
		return ;
	}

	va_list list;
	va_start(list, format);
	link->api->put(link, format, list);
	va_end(list);
}

// ============================================================================
//  Open Nodes
// ============================================================================

link_t lt_open(link_t* node, char* path) {
	if ( node == NULL || node->api == NULL || node->api->open_attr == NULL ) {
		return lt_make_error("Link is null");
	}

	link_t file;
	node->api->open_attr(node, &file, path);
	return file;
}

void lt_link_open(link_t* node, link_t* out, char* path) {
	if ( node == NULL || node->api == NULL || node->api->open_attr == NULL ) {
		return lt_make_error("Link is null");
	}

	node->api->open_attr(node, out, path);
}

void lt_close(link_t* node) {
	if ( node == NULL || node->api == NULL || node->api->close == NULL ) {
		return lt_make_error("Link is null");
	}
	node->api->close(node);
}

link_t lt_open_file(link_t* node, char* path) {
	if ( node == NULL || node->api == NULL || node->api->open_attr == NULL ) {
		return lt_make_error("Link is null");
	}

	link_t file;
	node->api->open_attr(node, &file, path);
	return file;
}

link_t lt_open_dir(link_t* node, char* path) {
	if ( node == NULL || node->api == NULL || node->api->open_attr == NULL ) {
		return lt_make_error("Link is null");
	}

	link_t dir;
	node->api->open_attr(node, &dir, path);
	return dir;
}

// ============================================================================
//  Iterators
// ============================================================================

link_t lt_begin(link_t* unit, char* path) {
	if ( unit == NULL || unit->api == NULL || unit->api->open_iter == NULL) {
		return lt_make_error("Link is invalid");
	}

	link_t iter;
	unit->api->open_iter(unit, &iter, path);
	return iter;
}

bool lt_next(link_t* iter, link_t* key, link_t* val) {
	if ( iter == NULL || iter->api == NULL || iter->api->next == NULL) {
		*key = lt_make_error("Link is invalid");
		*val = lt_make_error("Link is invalid");
		return false;
	}

	return iter->api->next(iter, key, val);
}



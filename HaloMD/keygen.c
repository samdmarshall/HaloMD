// Jonathan wrote this

#include "keygen.h"

// All blacklisted keys, minus 6 invalid keys and a duplicate
static const uint8_t blacklist[][8] = {
	{0x8d, 0x25, 0xc1, 0x44, 0xfa, 0x96, 0x4a, 0xdc},
	{0xf7, 0x3f, 0xd5, 0xa0, 0x06, 0xec, 0x3a, 0xd9},
	{0x77, 0xe1, 0xdd, 0xfd, 0xc6, 0x32, 0x73, 0xdd},
	{0xde, 0x3e, 0xf2, 0x15, 0x6c, 0x39, 0x17, 0x48},
	{0xa6, 0xa1, 0x90, 0x87, 0xbf, 0x66, 0x98, 0x3e},
	{0x45, 0xa4, 0x97, 0xc6, 0x91, 0xed, 0x60, 0x50},
	{0x03, 0xfd, 0x00, 0x3b, 0x93, 0xed, 0x78, 0xf7},
	{0xea, 0xd1, 0x56, 0xba, 0x3c, 0x81, 0x65, 0x33},
	{0x0c, 0xba, 0xac, 0x40, 0x27, 0xa9, 0x25, 0x5e},
	{0x82, 0x37, 0x61, 0x1b, 0x78, 0x23, 0x85, 0xde},
	{0xdf, 0x8f, 0x4b, 0x4a, 0xba, 0x58, 0x06, 0xa7},
	{0x64, 0x38, 0xe6, 0x7c, 0xec, 0x8a, 0xa9, 0xf9},
	{0x07, 0xa0, 0x39, 0x24, 0x73, 0x71, 0x93, 0x05},
	{0xe1, 0x49, 0x21, 0x45, 0x49, 0x19, 0x82, 0xea},
	{0x8b, 0xe0, 0xa9, 0x1a, 0x87, 0xc4, 0xa2, 0xab},
	{0xf6, 0x77, 0x43, 0x63, 0xde, 0x52, 0xea, 0x5a},
	{0x69, 0x7e, 0x48, 0xd4, 0x2c, 0xb7, 0x7b, 0xa5},
	{0xfc, 0x9b, 0xd6, 0x98, 0x46, 0x53, 0xa0, 0x86},
	{0x18, 0x5e, 0x15, 0x3d, 0xb5, 0x78, 0x84, 0xee},
	{0xac, 0xa2, 0x85, 0x14, 0xbd, 0x97, 0x92, 0x21},
	{0x1c, 0x9b, 0xce, 0x5a, 0x09, 0x42, 0x8e, 0xe3},
	{0xb5, 0xb0, 0x5e, 0xe7, 0x75, 0x2a, 0xac, 0x24},
	{0x8b, 0xe5, 0xfe, 0x78, 0x72, 0xc4, 0x5f, 0x57},
	{0x62, 0x21, 0xa8, 0x13, 0x65, 0xbc, 0xd5, 0xfc},
	{0xfd, 0x42, 0x83, 0x01, 0x56, 0x27, 0x4b, 0x66},
	{0xbc, 0xff, 0xb0, 0x76, 0x9e, 0x91, 0xab, 0x50},
	{0xa0, 0xcb, 0xaa, 0xef, 0x6b, 0xea, 0xbc, 0x5c},
	{0xde, 0xf9, 0x17, 0x05, 0xd5, 0xc2, 0xc3, 0x85},
	{0xd7, 0x30, 0x7c, 0x68, 0x68, 0xaa, 0x6b, 0xd6},
	{0xb9, 0x72, 0x04, 0x61, 0xbc, 0x76, 0x3d, 0xf3},
	{0xde, 0x0f, 0x93, 0xd7, 0x31, 0xc2, 0xba, 0xf4},
	{0x75, 0x2d, 0x72, 0x14, 0x9d, 0x07, 0x47, 0x1e},
	{0xe1, 0xde, 0x89, 0x28, 0xe2, 0x34, 0x48, 0x21},
	{0x8f, 0xab, 0xb1, 0x47, 0xf6, 0x91, 0xcf, 0xe1},
	{0xc8, 0x80, 0xb0, 0x30, 0xc5, 0x2a, 0x4d, 0x98},
	{0x46, 0xef, 0x2f, 0xa5, 0x03, 0xbb, 0x23, 0x24},
	{0xa3, 0xb0, 0x6f, 0x4c, 0x37, 0x1c, 0x74, 0x31},
	{0x6c, 0x68, 0x84, 0x81, 0xac, 0x04, 0x59, 0x53},
	{0x1f, 0x2b, 0x9c, 0x8e, 0xb6, 0x19, 0x1c, 0xb5},
	{0xee, 0x6b, 0xf7, 0x9b, 0xfc, 0xc9, 0xdf, 0x33},
	{0x05, 0x6d, 0x89, 0xab, 0xba, 0x40, 0x20, 0x3f},
	{0xbc, 0x31, 0x77, 0x5a, 0xe5, 0xc2, 0x86, 0xb4},
	{0x77, 0xdc, 0xb5, 0x24, 0xad, 0x43, 0x4f, 0xdb},
	{0x68, 0x0c, 0x24, 0x32, 0x7c, 0x41, 0x26, 0x4a},
	{0x7c, 0x24, 0x85, 0xa0, 0x94, 0xa2, 0x3f, 0xef},
	{0xed, 0x40, 0x41, 0x1a, 0x07, 0x5e, 0x0f, 0x1e},
	{0x11, 0xa3, 0xd5, 0x68, 0xfb, 0x1b, 0x04, 0x92},
	{0xfb, 0xb7, 0x3b, 0x7e, 0x06, 0xc5, 0xda, 0x26},
	{0x30, 0x8a, 0x74, 0x52, 0x88, 0xe0, 0x32, 0x2e},
	{0xf7, 0x84, 0xf9, 0x15, 0xfd, 0xc2, 0x2a, 0xca},
	{0x7c, 0xe6, 0x98, 0x36, 0x86, 0x63, 0xd7, 0x54},
	{0x86, 0xf5, 0x04, 0xc9, 0x53, 0x8f, 0x87, 0x22},
	{0x74, 0x26, 0x1d, 0xf3, 0x7e, 0x98, 0x97, 0x3f},
	{0x40, 0xe1, 0x35, 0x2b, 0x7f, 0xe8, 0xfe, 0xe7},
	{0x35, 0x28, 0xa1, 0x3c, 0xe1, 0xb0, 0x08, 0x0b},
	{0x8d, 0xf6, 0x6a, 0x0a, 0x0f, 0xa4, 0x13, 0xd9},
	{0xdd, 0xd1, 0x28, 0x07, 0xdf, 0x19, 0x4a, 0x2b},
	{0xd8, 0x99, 0x62, 0x82, 0x4c, 0x37, 0x18, 0xb2},
	{0x09, 0xcd, 0x32, 0x8c, 0xb5, 0xcc, 0x6a, 0x94},
	{0xe4, 0x47, 0x7b, 0x49, 0xdc, 0xad, 0xb2, 0xb9},
	{0x38, 0xa0, 0xe6, 0x13, 0x63, 0xd9, 0xe0, 0x0e},
	{0x22, 0xca, 0x5c, 0x0b, 0x48, 0x49, 0xd7, 0x39},
	{0x16, 0x17, 0x00, 0x31, 0x9c, 0xf7, 0x9b, 0x4e},
	{0xd5, 0x2a, 0xbb, 0x8c, 0x0e, 0xc2, 0xe6, 0xa7},
	{0x56, 0xc5, 0x9e, 0x6a, 0x02, 0x96, 0xbc, 0xa0},
	{0xfe, 0x84, 0x6d, 0xcc, 0x82, 0xe8, 0x01, 0x8a},
	{0x81, 0x82, 0xb9, 0xc3, 0x46, 0xe2, 0x82, 0x59},
	{0x0d, 0x31, 0x2f, 0xc3, 0xa7, 0x47, 0x8c, 0x83},
	{0xea, 0x28, 0x81, 0x7d, 0x27, 0x41, 0x2b, 0x99},
	{0xe4, 0xcc, 0xc3, 0x19, 0x68, 0x47, 0xe1, 0x04},
	{0x4e, 0x16, 0x73, 0xa3, 0x8e, 0xa0, 0xbd, 0xec},
	{0x15, 0x1f, 0x42, 0xc4, 0xc8, 0x60, 0x19, 0x08},
	{0xba, 0xa7, 0xad, 0x98, 0x1f, 0xfa, 0xe7, 0x70},
	{0x30, 0x5c, 0x76, 0xb2, 0x19, 0xfb, 0xdb, 0xa2},
	{0x5a, 0x4a, 0x00, 0xb6, 0x17, 0xe3, 0x01, 0x16},
	{0x91, 0xbd, 0x3d, 0x1d, 0xe1, 0x47, 0xe2, 0x0c},
	{0x79, 0xd0, 0x36, 0x51, 0x3c, 0x04, 0x83, 0x94},
	{0x4d, 0x44, 0x96, 0xb7, 0xf4, 0x64, 0x77, 0xbf},
	{0x7b, 0x2f, 0x21, 0x3a, 0x94, 0x91, 0xb7, 0x91},
	{0xbb, 0x3c, 0x14, 0xbf, 0x7b, 0xd6, 0xfd, 0xeb},
	{0x15, 0x96, 0xe6, 0xc3, 0x5e, 0xfc, 0x99, 0x85},
	{0x4e, 0xa4, 0xa9, 0x2e, 0xd9, 0x32, 0x99, 0xa8},
	{0x13, 0x80, 0xa3, 0x70, 0x7d, 0x50, 0xae, 0xe5},
	{0x53, 0x78, 0x29, 0x20, 0x50, 0xfb, 0xbe, 0x55},
	{0x95, 0x9c, 0xd5, 0x6e, 0x43, 0x18, 0x6a, 0xc2},
	{0x4a, 0xa4, 0xa7, 0x14, 0xaf, 0x0d, 0x96, 0xcb},
	{0x1e, 0x8f, 0x9b, 0xb4, 0x1d, 0xe6, 0x7f, 0x41},
	{0xdf, 0x2c, 0x3d, 0x40, 0x64, 0xa4, 0x1d, 0x28},
	{0x04, 0x03, 0xf7, 0x2b, 0x28, 0xc0, 0x63, 0x90},
	{0x26, 0x7f, 0x15, 0x71, 0x00, 0xdf, 0x46, 0x41},
	{0x27, 0xc3, 0xfb, 0x92, 0x74, 0x3c, 0x8c, 0x33},
	{0xb3, 0x93, 0xf0, 0x8d, 0xce, 0x6e, 0x34, 0xcf},
	{0x07, 0xce, 0x8d, 0xf8, 0x09, 0xdb, 0x22, 0x6b},
	{0x2e, 0x5d, 0x0f, 0x14, 0xe0, 0xe8, 0xbf, 0x08},
	{0x78, 0xcd, 0x7f, 0xeb, 0x42, 0x2f, 0xb2, 0xd7},
	{0x75, 0x26, 0x10, 0x01, 0x70, 0xb0, 0x7f, 0xdc},
	{0x42, 0xbd, 0xcd, 0x22, 0x47, 0x66, 0xf9, 0xf1},
	{0xa6, 0xfa, 0x32, 0xd5, 0x18, 0xbb, 0x34, 0xaf},
	{0x2e, 0xea, 0x0e, 0xb6, 0x5a, 0x5a, 0x0b, 0x1c},
	{0xa1, 0x4d, 0x10, 0x63, 0xa9, 0x45, 0x56, 0xf6},
	{0xea, 0xfa, 0xc1, 0x07, 0x61, 0xe8, 0x70, 0xee},
	{0xc0, 0xd5, 0xec, 0xc5, 0x71, 0xc9, 0xf1, 0xd9},
	{0xc3, 0xa7, 0x27, 0xd8, 0x4d, 0xac, 0x64, 0xf2},
	{0xe8, 0xd7, 0x01, 0xaf, 0x94, 0x7a, 0xe6, 0x39},
	{0x3f, 0xfd, 0x0a, 0xf1, 0xf1, 0xbb, 0xc8, 0x56},
	{0x97, 0x3b, 0xed, 0xd0, 0xc8, 0x3d, 0xfe, 0xed},
	{0x64, 0x98, 0x5a, 0x1a, 0xe9, 0x79, 0xfa, 0x7e},
	{0xcd, 0xb6, 0xb0, 0x1f, 0xa7, 0x7c, 0xb3, 0x71},
	{0x64, 0xd1, 0x3b, 0xd3, 0x02, 0x68, 0xf8, 0x5f},
	{0xe7, 0x70, 0x6b, 0xf4, 0x6f, 0x87, 0x79, 0x35},
	{0x4c, 0x04, 0xd8, 0x5b, 0x4e, 0x8e, 0x7a, 0xcd},
	{0xc0, 0x46, 0x45, 0x4b, 0xd4, 0xee, 0x3a, 0xa9},
	{0x77, 0x5f, 0xfe, 0xaf, 0x64, 0xbc, 0x22, 0x00},
	{0xe7, 0x91, 0xd0, 0x93, 0xed, 0x41, 0x45, 0x9c},
	{0x47, 0xfe, 0xea, 0xdc, 0xf5, 0x8b, 0x6a, 0x7a},
	{0x8d, 0x0a, 0xeb, 0x6f, 0x8d, 0x85, 0x0b, 0x9a},
	{0xd4, 0xe5, 0xef, 0x09, 0x73, 0xf7, 0xf3, 0xd7},
	{0xe2, 0x51, 0x2f, 0x94, 0xee, 0x41, 0x35, 0x26},
	{0xb3, 0x55, 0x75, 0x85, 0xf7, 0x8d, 0x3e, 0x6d},
	{0xfa, 0xf7, 0x3a, 0x45, 0xd8, 0x6f, 0x7c, 0xf5},
	{0x1e, 0xd5, 0x0c, 0x30, 0x54, 0x8b, 0xad, 0x31},
	{0x65, 0x36, 0x87, 0x5a, 0x4f, 0x85, 0x7e, 0x2b},
	{0xc8, 0x5c, 0x90, 0x55, 0x5e, 0xdf, 0xba, 0xdf},
	{0x31, 0x90, 0xe6, 0xf4, 0xad, 0xc5, 0xe8, 0xd3},
	{0x36, 0x07, 0xbc, 0x6d, 0x9b, 0x66, 0xcc, 0xca},
	{0x2c, 0x7e, 0x70, 0x9f, 0xec, 0x28, 0x6c, 0x81},
	{0x05, 0x89, 0x73, 0x6a, 0x58, 0xc2, 0x36, 0xc5},
	{0xfa, 0xcc, 0x95, 0x02, 0x11, 0x7c, 0x39, 0x11},
	{0xab, 0xe2, 0x2d, 0xc3, 0x46, 0xa0, 0x4e, 0x50},
	{0x83, 0xd3, 0x7a, 0xbb, 0x30, 0xe5, 0x9a, 0x47},
	{0x20, 0x9c, 0x3b, 0x17, 0x56, 0xc3, 0xc0, 0xcb},
	{0x74, 0xf0, 0x14, 0xb1, 0x92, 0xc6, 0xbf, 0xff},
	{0xcf, 0x90, 0x7e, 0xb8, 0x24, 0xf9, 0x74, 0xf1},
	{0xed, 0x80, 0x4a, 0x01, 0xd7, 0xbd, 0x63, 0x8b},
	{0xa5, 0xfe, 0x9c, 0xcc, 0xb5, 0x9c, 0x65, 0xf7},
	{0x1e, 0x1b, 0xcd, 0x72, 0xec, 0xe6, 0x76, 0xa3},
	{0x7c, 0x76, 0x6a, 0x6a, 0x59, 0x3b, 0x0e, 0xbd},
	{0x17, 0x1d, 0x23, 0xd6, 0x7f, 0xf2, 0x86, 0x4f},
	{0xd5, 0xde, 0x08, 0x82, 0xe0, 0xf2, 0x34, 0xa1},
	{0xd2, 0x39, 0x70, 0xce, 0x32, 0x4b, 0xc8, 0x9d},
	{0xf4, 0x1e, 0x74, 0xaa, 0x3b, 0xf1, 0x5f, 0x3e},
	{0xb4, 0x2b, 0x9d, 0xcd, 0xdf, 0xbd, 0xb4, 0x90},
	{0xb5, 0xdf, 0x4d, 0xb9, 0x66, 0x83, 0x06, 0xe3},
	{0xee, 0xc6, 0xb1, 0x33, 0x70, 0x21, 0x31, 0xf7},
	{0x04, 0x34, 0xb9, 0x18, 0x8b, 0x96, 0x99, 0x3d},
	{0x1e, 0x60, 0x03, 0x2a, 0x88, 0x4d, 0x33, 0x2b},
	{0x7e, 0x54, 0xc5, 0x71, 0xea, 0x92, 0x54, 0x97},
	{0xae, 0x38, 0xae, 0xc9, 0x69, 0x70, 0xcd, 0x92},
	{0x82, 0xb2, 0xa8, 0x35, 0x53, 0x79, 0xd6, 0xfb},
	{0xaa, 0x60, 0xb8, 0x43, 0x02, 0x9e, 0x72, 0x5e},
	{0xf3, 0x05, 0x3a, 0x9d, 0xfe, 0x8d, 0x2c, 0x39},
	{0x97, 0xc7, 0x81, 0x4e, 0x04, 0x15, 0xd7, 0x9e},
	{0x66, 0x9c, 0xae, 0x76, 0x62, 0xb7, 0xf4, 0x35},
	{0x9f, 0xc6, 0x6e, 0x25, 0x89, 0x42, 0x3d, 0x1b},
	{0x13, 0xfc, 0xf8, 0x89, 0x99, 0x6d, 0xd1, 0x82},
	{0xd5, 0xfb, 0xd6, 0x06, 0xac, 0x3a, 0xac, 0x1c},
	{0x8c, 0xc8, 0x2e, 0xdd, 0xf7, 0x59, 0xfa, 0xc9},
	{0xb6, 0x4b, 0x5f, 0x46, 0xa9, 0x92, 0xb7, 0x3f},
	{0x5a, 0x83, 0x49, 0x69, 0x5f, 0x20, 0x8f, 0xbb},
	{0x76, 0xd1, 0x7a, 0x87, 0x54, 0x93, 0x5c, 0x81},
	{0x72, 0x49, 0x3a, 0x2b, 0xa4, 0xe2, 0x7a, 0x8b},
	{0xbf, 0xa1, 0x92, 0x83, 0xb9, 0xa0, 0x4b, 0xa8},
	{0x76, 0x27, 0xc5, 0x1c, 0x1b, 0x7a, 0x63, 0xe8},
	{0xe3, 0xee, 0xc5, 0xd8, 0x5e, 0x67, 0x75, 0x2e},
	{0x8c, 0x02, 0x06, 0x70, 0x36, 0xb4, 0xdf, 0xa5},
	{0x73, 0x7c, 0x38, 0xf3, 0x65, 0x38, 0x08, 0xe7},
	{0xc5, 0xbb, 0x70, 0x2f, 0xa4, 0x70, 0xb8, 0xa4},
	{0x8b, 0xd2, 0xf3, 0x27, 0x49, 0x2c, 0x4b, 0x32},
	{0xb4, 0xf4, 0xc5, 0x58, 0x17, 0x87, 0x2c, 0xe6},
	{0xa5, 0xd6, 0x7e, 0x1a, 0x3c, 0xce, 0x7d, 0x6b},
	{0xac, 0xa6, 0xb1, 0x3f, 0x4a, 0xdd, 0x20, 0x6c},
	{0x06, 0x90, 0x8f, 0x52, 0x9d, 0xba, 0x4b, 0x7e},
	{0x15, 0x0a, 0x78, 0x3a, 0x88, 0x19, 0x70, 0xb5},
	{0x6e, 0x16, 0x4c, 0x96, 0x5f, 0x2e, 0x02, 0x17},
	{0xec, 0xdc, 0x04, 0xad, 0xda, 0x64, 0xdc, 0xc9},
	{0x00, 0x7d, 0x94, 0xfe, 0xe7, 0xf3, 0x33, 0xc9},
	{0xf9, 0x86, 0xb6, 0x87, 0x3f, 0xbd, 0x44, 0xe4},
	{0x46, 0xc7, 0x24, 0x0a, 0xa2, 0xae, 0x10, 0x33},
	{0x66, 0xb1, 0x02, 0x68, 0x43, 0xd7, 0x63, 0x82},
	{0x4a, 0xc3, 0xb0, 0x59, 0x1f, 0xf9, 0x42, 0xbf},
	{0xc1, 0xf1, 0x64, 0x04, 0x2d, 0x59, 0x73, 0xb9},
	{0x9b, 0xcd, 0xa3, 0xfe, 0x8a, 0x10, 0xfb, 0x70},
	{0xa3, 0xd6, 0xa1, 0x7d, 0x9d, 0xff, 0x0e, 0xd4},
	{0x2b, 0x13, 0x9f, 0x55, 0xd8, 0x6b, 0x3a, 0x09},
	{0x42, 0xcb, 0xaa, 0x29, 0x98, 0xb0, 0xdc, 0x88},
	{0x9b, 0x07, 0xae, 0xe7, 0xf6, 0x1a, 0x5b, 0xa9},
	{0x60, 0xad, 0xd6, 0xa7, 0xb9, 0xc6, 0x5b, 0xf4},
	{0xeb, 0x99, 0x45, 0x58, 0x3b, 0xb3, 0x55, 0x14},
	{0xa3, 0x22, 0x62, 0x22, 0xd7, 0x1a, 0x9e, 0x5c},
	{0x43, 0x62, 0x54, 0x57, 0x38, 0x72, 0x39, 0x1a},
	{0xd5, 0xb8, 0x24, 0x1e, 0x39, 0xb0, 0x1a, 0xf8},
	{0xab, 0xad, 0x9a, 0x75, 0x38, 0x41, 0x5e, 0x12},
	{0x78, 0xe4, 0x82, 0xa0, 0x3a, 0x31, 0x6e, 0x8b},
	{0x72, 0x2a, 0xce, 0x37, 0xa4, 0xa1, 0xa7, 0xac},
	{0x46, 0x7a, 0xc4, 0x5c, 0xa8, 0xae, 0x30, 0xfa},
	{0x06, 0x47, 0x27, 0x78, 0x22, 0x88, 0x1e, 0xb1},
	{0x7f, 0xa3, 0xff, 0x4e, 0xb7, 0xb7, 0x94, 0x01},
	{0x82, 0x98, 0xeb, 0x06, 0xa4, 0x4a, 0xf9, 0xf2},
	{0x10, 0xb3, 0xf1, 0x5b, 0x42, 0x97, 0x6f, 0x66},
	{0xd2, 0xc4, 0xa1, 0xc6, 0x8d, 0xf2, 0x77, 0x97},
	{0xc1, 0x3d, 0x66, 0xee, 0xa6, 0xb4, 0x90, 0xc0},
	{0xa7, 0x3e, 0x57, 0x4c, 0x2e, 0xde, 0x92, 0x77},
	{0x89, 0x23, 0x9f, 0x1a, 0xd5, 0x63, 0xe0, 0xc0},
	{0x72, 0xd0, 0xc1, 0xae, 0xdf, 0x3f, 0xb2, 0x61},
	{0xc5, 0x27, 0x4a, 0x1e, 0x64, 0x80, 0xbc, 0x8c},
	{0x22, 0x07, 0x50, 0x73, 0x14, 0x7d, 0x15, 0x17},
	{0x90, 0xc8, 0x4c, 0x55, 0x9e, 0x7b, 0xaa, 0x05},
	{0x4d, 0xe5, 0x37, 0x6c, 0x10, 0x95, 0x40, 0xb6},
	{0x2d, 0x52, 0x43, 0x62, 0x4f, 0xc3, 0x50, 0x9a},
	{0xd4, 0x71, 0xa9, 0x24, 0x0b, 0x50, 0x7b, 0x5c},
	{0x43, 0x0d, 0xb7, 0x68, 0x3a, 0x5c, 0x71, 0x86},
	{0xf3, 0x45, 0x8b, 0x22, 0x37, 0x1f, 0x28, 0x89},
	{0x57, 0xf3, 0xbd, 0x91, 0xff, 0x03, 0xf5, 0xff},
	{0x18, 0x45, 0x76, 0x70, 0x6d, 0xaf, 0x8a, 0x1e},
	{0x72, 0xc9, 0xa3, 0x81, 0x91, 0xea, 0x23, 0x91},
	{0x0d, 0x6a, 0xdf, 0x84, 0xe2, 0x38, 0xb0, 0xc0},
	{0xf9, 0x7c, 0x86, 0x35, 0x4e, 0x38, 0x60, 0xc0},
	{0x60, 0x99, 0x49, 0x4a, 0x11, 0x9b, 0x2b, 0x24},
	{0xaa, 0xfd, 0x99, 0x38, 0x36, 0xd3, 0xd4, 0xe1},
	{0x3a, 0xe8, 0xee, 0xbd, 0x5d, 0x3f, 0xa3, 0xa7},
	{0x86, 0xa1, 0x2f, 0x5e, 0xcc, 0xc0, 0x7f, 0x1f},
	{0x16, 0x08, 0xf3, 0xc4, 0x53, 0xfd, 0x67, 0x42},
	{0x50, 0x5a, 0xae, 0x89, 0x30, 0x5f, 0x07, 0xba},
	{0x70, 0x9e, 0x40, 0xf0, 0xc8, 0xca, 0x15, 0x3e},
	{0x71, 0xf7, 0x78, 0x5f, 0xe8, 0x02, 0x8d, 0xdf},
	{0x11, 0x1f, 0xb7, 0x3e, 0x67, 0xd1, 0x4a, 0x39},
	{0x35, 0xd9, 0xa7, 0x4e, 0x47, 0x98, 0x73, 0xff},
	{0x75, 0xba, 0xc6, 0x9a, 0xba, 0xb7, 0xc2, 0xbb},
	{0x55, 0xa3, 0xdc, 0x4f, 0x94, 0xce, 0xf5, 0xee},
	{0x6a, 0xa6, 0xc2, 0xe2, 0xed, 0x34, 0xb5, 0x44},
	{0xf7, 0x19, 0x46, 0x64, 0x6d, 0x4b, 0xcc, 0xe7},
	{0xbc, 0x37, 0x41, 0xff, 0x0e, 0x3e, 0xb4, 0x37},
	{0x20, 0xab, 0xd2, 0xea, 0x84, 0x76, 0x75, 0x5b},
	{0x87, 0x60, 0x0e, 0x0c, 0x0b, 0x60, 0x37, 0x4b},
	{0x31, 0xa7, 0x94, 0x9d, 0x30, 0x0e, 0xfe, 0x3b},
	{0x10, 0x6b, 0x6c, 0x50, 0x13, 0xb5, 0xff, 0x27},
	{0x41, 0xa5, 0xf6, 0x4d, 0x4c, 0x5a, 0x5b, 0xac},
	{0x03, 0x3b, 0x26, 0xe7, 0x8f, 0x9b, 0x2e, 0x4d},
	{0x26, 0x96, 0x99, 0x91, 0xcf, 0xd3, 0x9d, 0x58},
	{0x32, 0x56, 0x8c, 0x58, 0x9c, 0xb2, 0xb9, 0xa1},
	{0xe6, 0xe1, 0x51, 0x19, 0x4c, 0xf1, 0xff, 0xed},
	{0xc3, 0xcc, 0x20, 0xdf, 0xbe, 0xcd, 0x67, 0xb2},
	{0xa4, 0x4b, 0x9c, 0x98, 0x51, 0x5e, 0xf6, 0xe3},
	{0x7c, 0x1c, 0x61, 0x4e, 0x46, 0x10, 0x68, 0x8c},
	{0x3c, 0x03, 0xac, 0xd1, 0x06, 0x76, 0x40, 0x2f},
	{0x11, 0xaf, 0x69, 0xa9, 0x23, 0xb3, 0xd8, 0xed},
	{0x5b, 0x0b, 0xad, 0x60, 0x67, 0x02, 0x75, 0x1a},
	{0xb2, 0xa7, 0x76, 0x90, 0x31, 0xbb, 0x93, 0x83},
	{0xc9, 0xc9, 0x35, 0x5d, 0x34, 0xaf, 0x73, 0x48},
	{0x20, 0xd5, 0x2b, 0x35, 0x2a, 0x8c, 0x1e, 0x23},
	{0xaf, 0x57, 0xde, 0xd4, 0x59, 0x3b, 0xc0, 0x1b},
	{0x7a, 0x6c, 0x58, 0xbf, 0x69, 0x8c, 0xe7, 0x5a},
	{0x67, 0x74, 0x46, 0x18, 0xcf, 0xb5, 0xa6, 0x98},
	{0x1e, 0x5c, 0x80, 0x2c, 0x3f, 0xf7, 0x42, 0x9a},
	{0xf5, 0xcc, 0x0f, 0xbd, 0x9b, 0x57, 0x30, 0x8c},
	{0xca, 0x13, 0xce, 0x61, 0xbf, 0x4e, 0x1f, 0xaa},
	{0x98, 0xe2, 0x1d, 0x1f, 0x03, 0x21, 0xbf, 0xce},
	{0xdd, 0x81, 0x25, 0xfa, 0xf2, 0x82, 0xd4, 0x2d},
	{0x70, 0x69, 0x1d, 0x77, 0x2f, 0x4b, 0x36, 0xcf},
	{0xd7, 0x41, 0xc2, 0x9e, 0x8a, 0x16, 0xd6, 0xe4},
	{0x2e, 0x83, 0x18, 0xc2, 0x49, 0x9a, 0xa6, 0xc8},
	{0x3a, 0x54, 0x83, 0x70, 0x4f, 0xcf, 0x92, 0x6b},
	{0xc4, 0xb4, 0x9a, 0x34, 0xa2, 0xe2, 0x6d, 0x10},
	{0xc9, 0x1a, 0xd6, 0x9e, 0x5f, 0x38, 0xcd, 0xb7},
	{0x84, 0xe7, 0x21, 0x3d, 0xff, 0x7b, 0x73, 0x3a},
	{0x97, 0x61, 0x42, 0x28, 0x81, 0x0c, 0x68, 0x41},
	{0x10, 0xb1, 0x66, 0x6c, 0x39, 0x8c, 0x7a, 0x0c},
	{0x2e, 0xc8, 0x09, 0x34, 0xba, 0x7f, 0x9b, 0xeb},
	{0x19, 0x53, 0xd6, 0x10, 0x1d, 0x8d, 0x32, 0x72},
	{0xcb, 0x0e, 0x3c, 0xc2, 0x2e, 0x04, 0x7d, 0xad},
	{0x8c, 0x7d, 0xa7, 0xc9, 0xc2, 0xd1, 0xd4, 0xdc},
	{0x7f, 0x66, 0x57, 0x8f, 0x3d, 0xb7, 0x13, 0xe8},
	{0xaf, 0xd0, 0x97, 0x87, 0xe8, 0x9c, 0x29, 0x38},
	{0x6c, 0x1a, 0xbe, 0x90, 0xf9, 0x99, 0x7f, 0xad},
	{0xaf, 0x5c, 0x30, 0xbe, 0x94, 0xaf, 0x76, 0xaa},
	{0x55, 0x90, 0xa8, 0x50, 0xcb, 0xde, 0x7e, 0xa5},
	{0x10, 0xc1, 0xff, 0x27, 0x2e, 0xf0, 0xe3, 0x9e},
	{0x44, 0xd3, 0xe9, 0xaa, 0xe1, 0xe5, 0xfc, 0x00},
	{0x59, 0xbe, 0xeb, 0x18, 0x44, 0x03, 0xe6, 0x27},
	{0x62, 0xb5, 0x84, 0xe7, 0x74, 0x27, 0x5a, 0x45},
	{0x65, 0xf8, 0xcc, 0x7d, 0xf8, 0x3b, 0x7d, 0xa9},
	{0xb9, 0x64, 0x7e, 0x29, 0xf5, 0x1e, 0x7e, 0x81},
	{0xef, 0xe4, 0x0c, 0x4d, 0xbf, 0xbe, 0x53, 0xad},
	{0x30, 0x2c, 0x8e, 0x71, 0xae, 0x97, 0xc7, 0xe7},
	{0x5f, 0x38, 0xee, 0x4a, 0xee, 0x12, 0x59, 0x9f},
	{0xd1, 0x91, 0x30, 0x51, 0x33, 0x96, 0x19, 0x8f},
	{0xb2, 0x8e, 0x16, 0x1d, 0xab, 0x7c, 0xb3, 0xf8},
	{0x6b, 0x28, 0x49, 0xa1, 0x0f, 0xf7, 0xa4, 0x4d},
	{0x2f, 0x3b, 0xac, 0x00, 0x82, 0xb6, 0x93, 0xad},
	{0xe6, 0x40, 0x86, 0xfa, 0xf0, 0x64, 0xe0, 0xc2},
	{0xc8, 0xfc, 0xbe, 0xbd, 0x2d, 0xc4, 0x92, 0x80},
	{0x59, 0x71, 0x98, 0xf9, 0x32, 0x68, 0xfa, 0xb6},
	{0xc9, 0x52, 0x63, 0x41, 0x61, 0x9e, 0x55, 0x9b},
	{0xbb, 0x55, 0x63, 0x47, 0x6f, 0x4c, 0x23, 0x46},
	{0x24, 0x90, 0x6b, 0x27, 0xef, 0xaf, 0x1c, 0x88},
	{0xf5, 0x29, 0x84, 0xdc, 0x9d, 0x31, 0xba, 0x17},
	{0xda, 0xde, 0x84, 0x7f, 0x6b, 0xce, 0x27, 0x14},
	{0xa9, 0x5d, 0x7a, 0xc6, 0x59, 0x54, 0x51, 0x48},
	{0x41, 0x9f, 0xe4, 0x59, 0x72, 0x7e, 0xcc, 0xd3},
	{0x00, 0x6b, 0x8f, 0x9b, 0x99, 0xec, 0x59, 0x1b},
	{0xa1, 0x4b, 0xa4, 0x5a, 0x6a, 0x19, 0xf3, 0x9b},
	{0x29, 0xc0, 0xd3, 0xbf, 0x62, 0x50, 0xda, 0x90},
	{0x18, 0xc0, 0xb6, 0xba, 0x48, 0x58, 0x38, 0xc8},
	{0x78, 0xe3, 0x9b, 0x58, 0x26, 0x08, 0x85, 0xbf},
	{0xd5, 0x27, 0x3b, 0xb6, 0x7b, 0x2b, 0x14, 0x01},
	{0x03, 0xc3, 0x8c, 0xe5, 0x43, 0x2f, 0x13, 0x26},
	{0x6a, 0x74, 0x4a, 0x64, 0xf2, 0xe7, 0xcc, 0x59},
	{0xc0, 0xfe, 0xed, 0x41, 0x93, 0x7b, 0x30, 0xd8},
	{0xbe, 0xb1, 0x68, 0x22, 0xf8, 0x02, 0x88, 0x2e},
	{0xdf, 0x21, 0x5a, 0x6b, 0xc7, 0xea, 0xdb, 0xe2},
	{0xb7, 0x80, 0x0f, 0xcb, 0xd7, 0xaf, 0x81, 0x05},
	{0xd8, 0x4c, 0x30, 0x4e, 0x8a, 0xb2, 0x29, 0xa6},
	{0x7e, 0xf6, 0x95, 0xee, 0xae, 0xe4, 0x28, 0xc6},
	{0xe3, 0x1f, 0xb4, 0x62, 0x26, 0x8b, 0x9e, 0x7f},
	{0x82, 0x59, 0xc6, 0xe2, 0x50, 0xfb, 0x6e, 0x6c},
	{0xa0, 0xfa, 0x24, 0x58, 0x0a, 0x05, 0xca, 0xd5},
	{0x8f, 0x67, 0x41, 0x51, 0xb7, 0x52, 0xfc, 0x3c},
	{0x6f, 0x93, 0x84, 0xca, 0x36, 0xae, 0xee, 0x26},
	{0x4b, 0x0c, 0x6b, 0xd0, 0x88, 0x37, 0xce, 0xea},
	{0xc4, 0xe1, 0x4a, 0x2c, 0x5f, 0x49, 0xbd, 0xbe},
	{0xcd, 0x71, 0x4e, 0x10, 0x13, 0x1a, 0xd4, 0x24},
	{0xca, 0xd6, 0xf3, 0x26, 0x05, 0x93, 0x8d, 0xac},
	{0xc7, 0x20, 0x5e, 0x15, 0x41, 0xab, 0xf4, 0x67},
	{0xc5, 0xfe, 0x2f, 0x75, 0xc4, 0x4b, 0x56, 0x16},
	{0xb6, 0x6d, 0x3f, 0xbf, 0xbb, 0x30, 0x68, 0x86},
	{0x2b, 0x6f, 0x1a, 0xf3, 0x16, 0xcd, 0x06, 0x8c},
	{0x74, 0xc4, 0x69, 0x98, 0x80, 0x8f, 0x33, 0x6a},
	{0xe2, 0x99, 0x17, 0x38, 0xd1, 0x1c, 0x95, 0x90},
	{0x4a, 0x99, 0x72, 0xba, 0xd9, 0xf0, 0x5e, 0xc6},
	{0x60, 0xc1, 0xc7, 0x5d, 0xed, 0xcc, 0xae, 0xeb},
	{0xf5, 0xc0, 0xe6, 0x32, 0xbd, 0x47, 0xd7, 0xd4},
	{0x0a, 0xe8, 0x81, 0x90, 0xff, 0xc3, 0x35, 0xd8},
	{0xd9, 0x8d, 0xb5, 0x14, 0x44, 0x06, 0x6b, 0x28},
	{0x2c, 0x8c, 0xc6, 0xe5, 0xa1, 0xf8, 0xbf, 0x69},
	{0x3b, 0xac, 0xc5, 0xe0, 0x44, 0xb5, 0x88, 0x6c},
	{0x9d, 0x3d, 0x4e, 0x34, 0xab, 0x05, 0xdc, 0xf6},
	{0xea, 0x19, 0x7f, 0x70, 0x24, 0xee, 0x31, 0xbe},
	{0xe6, 0x6b, 0xfc, 0xa3, 0x81, 0xc9, 0x2d, 0x95},
	{0x1d, 0x34, 0xfc, 0xbd, 0xce, 0x97, 0xed, 0x82},
	{0x19, 0x6f, 0x35, 0x9d, 0x38, 0xdd, 0xc5, 0xac},
	{0xb3, 0x84, 0xe9, 0x59, 0x1d, 0x9a, 0x19, 0x2f},
	{0x50, 0x9f, 0x82, 0x90, 0x51, 0xf7, 0xbb, 0x42},
	{0x70, 0x26, 0x99, 0x52, 0x56, 0xec, 0xaf, 0xec},
	{0xed, 0x69, 0xbd, 0x45, 0x97, 0xba, 0xdd, 0x46},
	{0xf4, 0xe4, 0xbb, 0x4b, 0xd2, 0xda, 0x7c, 0xad},
	{0x1b, 0x8c, 0x3c, 0x6c, 0x34, 0xc3, 0x87, 0x0b},
	{0x83, 0x83, 0xfa, 0x74, 0xfa, 0x54, 0x6a, 0xe2},
	{0x01, 0x58, 0x97, 0xae, 0xaa, 0xc8, 0x2e, 0x91},
	{0x40, 0xbc, 0x72, 0xba, 0xb1, 0x31, 0xe3, 0x55}
};

// Get a non-blacklisted value from /dev/urandom
void generate_key(uint8_t *key)
{
	FILE *fp = fopen("/dev/urandom", "rb");
	if (!fp) {
		fprintf(stderr, "Where'd your /dev/urandom go?\n");
		exit(1);
	}
	repeat:
		if (fread(key, 1, 8, fp) != 8) {
			fprintf(stderr, "Your source of entropy broke!\n");
			exit(1);
		}
		for (size_t i = 0; i != sizeof blacklist / sizeof *blacklist; ++i)
			if (memcmp(key, blacklist[i], 8) == 0)
				goto repeat; // Multi-level continue
		// Break
	fclose(fp);
}

// Fix the checksum
void fix_key(uint8_t *key)
{
	uint32_t accum = 0;
	for (int i = 0; i != 8; ++i) {
		accum *= 0x9ccf9319U;
		accum += key[i];
	}
	accum %= 65521;
	accum ^= 0x319;
	key[8] = accum;
	key[9] = accum >> 8;
}

// Printed keys are in base 32
static const char digit_table[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

// A key consists of 80 bits: 10*8 internally or 16*5 as text
// dst must have room for 20 characters (19 + terminator)
void print_key(char *dst, const uint8_t *key)
{
	// Two groups of 40 bits
	for (int i = 0; i != 2; ++i) {
		uint64_t value = 0;
		// Load 5*8 bits
		for (int j = 5; j != 0;) { // Reversed loop
			value <<= 8;
			value |= key[--j];
		}
		// Dump 4*5 bits, twice, inserting '-' in between
		for (int j = 0; j != 2; ++j) {
			for (int j = 0; j != 4; ++j) {
				*dst++ = digit_table[value & 0x1f];
				value >>= 5;
			}
			*dst++ = '-';
		}
		key += 5;
	}
	dst[-1] = 0; // Replace final '-'
}

#ifdef KEYGEN_TESTING
int main(int argc, char **argv)
{
	uint8_t key[10];
	char ascii[20];
	generate_key(key);
	fix_key(key);
	print_key(ascii, key);
	printf("%s\n", ascii);
	return 0;
}
#endif

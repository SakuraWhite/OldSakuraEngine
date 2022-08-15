// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "../../../simple_library_macro.h"

struct SIMPLE_LIBRARY_API fvector_2id
{
	int x;
	int y;
public:
	fvector_2id();

	fvector_2id(int in_x, int in_y);

	fvector_2id(int in_value);
};
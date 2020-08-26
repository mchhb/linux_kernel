/*
 * (C) Copyright 2012 Stephen Warren
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#include <common.h>

int arch_cpu_init(void)
{
	icache_enable();

	return 0;
}

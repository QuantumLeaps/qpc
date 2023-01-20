//============================================================================
// Board Support Package (BSP) for the Calculator example
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
#ifndef BSP_H_
#define BSP_H_

void BSP_clear(void);
void BSP_negate(void);
void BSP_insert(uint8_t keyId);
float64_t BSP_get_value(void);
void BSP_display(float64_t value);
void BSP_display_error(char const *err);
void BSP_exit(void);

void BSP_show_display(void);
void BSP_message(char const *msg);

#endif // BSP_H_

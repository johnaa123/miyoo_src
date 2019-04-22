/*

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __CPUNO_H__
#define __CPUNO_H__

#ifdef __cplusplus
extern "C" {
#endif

# define GP2X_DEF_CLOCK     500
# define GP2X_DEF_EMU_CLOCK 500
# define GP2X_MIN_CLOCK      60
# define GP2X_MAX_CLOCK     800

# define cpu_init() 
# define cpu_deinit()

# define cpu_get_clock()   0
# define cpu_set_clock(N)

#ifdef __cplusplus
}
#endif

#endif

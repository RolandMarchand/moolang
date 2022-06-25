# Moolang
## About
Moolang is an educational programming language designed and written by
[Moowool L. Summerwheat](https://moowool.info/) to teach developers C
programming and compiler design.

Moolang aims for code quality first and foremost, and makes use of excellent
basic and advanced practices to optimize the educational value of the
codebase.
## Compiling
Moolang uses CMake and is written for Linux with glibc.
TODO: find the minimum compatible build.

To build, run those commands in order.
\code
cd /path/to/moolang
mkdir build
cmake -S . -B build
cd build
make -j$(nproc)
\endcode
## License
> Moolang, a programming language.<br />
> Copyright (C) 2022 moowool195@gmail.com
> 
> This program is free software: you can redistribute it and/or modify<br />
> it under the terms of the GNU General Public License as published by<br />
> the Free Software Foundation, either version 3 of the License, or<br />
> (at your option) any later version.
> 
> This program is distributed in the hope that it will be useful,<br />
> but WITHOUT ANY WARRANTY; without even the implied warranty of<br />
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br />
> GNU General Public License for more details.
> 
> You should have received a copy of the GNU General Public License<br />
> along with this program.  If not, see <https://www.gnu.org/licenses/>.

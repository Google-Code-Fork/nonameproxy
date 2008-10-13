/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

#ifndef __SAFE_FILE_H
#define __SAFE_FILE_H

//a simple class for preforming file io with error checking

#include <stdio.h>
#include <stdint.h>

class SafeFile
{
        public:
                enum FileMode {
                        m_read,
                        m_read_b,
                        m_read_p,
                        m_read_bp,
                        m_write,
                        m_write_b,
                        m_write_p,
                        m_write_bp,
                        m_append,
                        m_append_b,
                        m_append_p,
                        m_append_bp,
                };

                enum SeekMode {
                        s_from_start,
                        s_from_current,
                        s_from_end,
                };

                SafeFile ();
                SafeFile (const char* filename, FileMode mode = m_read);
                virtual ~SafeFile ();

                void open (const char* filename, FileMode mode = m_read);
                void close ();
                
                bool eof ();

                void seek (int32_t offset, SeekMode from);
                int32_t tell ();

                int32_t read (void* buffer, int32_t size);
                int32_t write (const void* buffer, int32_t size);
        private:
                FILE* _fp;
                bool  _isOpen;
                FileMode _mode;
};

#endif

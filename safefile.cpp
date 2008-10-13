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

#include <stdio.h>
#include "safefile.h"

SafeFile::SafeFile ()
{
        _fp = NULL;
        _isOpen = false;
        _mode = m_read;
}

SafeFile::SafeFile (const char* filename, FileMode mode)
{
        _fp = NULL;
        _isOpen = false;
        open (filename, mode);
}

SafeFile::~SafeFile ()
{
        if (_isOpen) {
                close ();
        }
}

void SafeFile::open (const char* filename, FileMode mode)
{
        if (_isOpen) {
                printf ("SafeFile warning: averted opening an open file\n");
                return;
        }
        if (mode == m_read) {
                _fp = fopen (filename, "r");
        } else if (mode == m_read_p) {
                _fp = fopen (filename, "r+");
        } else if (mode == m_write) {
                _fp = fopen (filename, "w");
        } else if (mode == m_write_p) {
                _fp = fopen (filename, "w+");
        } else if (mode == m_append) {
                _fp = fopen (filename, "a");
        } else if (mode == m_append_p) {
                _fp = fopen (filename, "a+");
        } else if (mode == m_read_b) {
                _fp = fopen (filename, "rb");
        } else if (mode == m_read_bp) {
                _fp = fopen (filename, "rb+");
        } else if (mode == m_write_b) {
                _fp = fopen (filename, "wb");
        } else if (mode == m_write_bp) {
                _fp = fopen (filename, "wb+");
        } else if (mode == m_append_b) {
                _fp = fopen (filename, "ab");
        } else if (mode == m_append_bp) {
                _fp = fopen (filename, "ab+");
        } else {
                printf ("SafeFile error: unknown FileMode\n");
                _fp = NULL;
        }

        if (_fp == NULL) {
                perror ("SafeFile error");
                _isOpen = false;
        } else {
                _isOpen = true;
                _mode = mode;
        }
}

void SafeFile::close ()
{
        if (!_isOpen) {
                printf ("SafeFile warning: averted closing an unopened file\n");
                return;
        } 
        if (fclose (_fp) != 0) {
                perror ("SafeFile error");
                return;
        }
        _isOpen = false;
}

bool SafeFile::eof ()
{
        if (!_isOpen) {
                printf ("SafeFile warning: averted eof on unopened file\n");
                return false;
        }
        if (_fp == NULL) {
                printf ("SafeFile error: eof FILE* is NULL\n");
        }
        return feof (_fp);
}

void SafeFile::seek (int32_t offset, SeekMode from)
{
        if (!_isOpen) {
                printf ("SafeFile warning: averted seek on unopened file\n");
                return;
        }
        if (_fp == NULL) {
                printf ("SafeFile error: seek FILE* is NULL\n");
        }
        int32_t whence;
        if (from == s_from_start) {
                whence = SEEK_SET;
        } else if (from == s_from_current) {
                whence = SEEK_CUR;
        } else if (from == s_from_end) {
                whence = SEEK_END;
        } else {
                printf ("SafeFile error: unknown seek mode\n");
                return;
        }
        
        if (fseek (_fp, offset, whence) != 0) {
                perror ("SafeFile error");
        }
}

int32_t SafeFile::tell ()
{
        if (!_isOpen) {
                printf ("SafeFile warning: averted tell on unopened file\n");
                return false;
        }
        if (_fp == NULL) {
                printf ("SafeFile error: tell FILE* is NULL\n");
        }

        int32_t offset = ftell (_fp);
        if (offset == -1) {
                perror ("SafeFile error:");
        }
        return offset;
}
        
int32_t SafeFile::read (void* buffer, int32_t size)
{
        if (!(_mode == m_read || _mode == m_read_p
                || _mode == m_read_b || _mode == m_read_bp))
        {
                printf ("SafeFile error: attempt to read with incorrect mode\n");
                return false;
        }
        if (!_isOpen) {
                printf ("SafeFile warning: averted read on unopened file\n");
                return false;
        }
        if (_fp == NULL) {
                printf ("SafeFile error: read FILE* is NULL\n");
        }

        int32_t readsize = fread (buffer, size, 1, _fp);

        if (readsize == -1 && !eof ()) {
                perror ("SafeFile error:");
        }
        return readsize;
}

int32_t SafeFile::write (const void* buffer, int32_t size)
{
        if (!(_mode == m_read_p || _mode == m_write || _mode == m_write_p 
                || _mode == m_append || _mode == m_append_p 
                || _mode == m_read_bp || _mode == m_write_b || _mode == m_write_bp 
                || _mode == m_append_b || _mode == m_append_bp))
        {
                printf ("SafeFile error: attempt to write with incorrect mode\n");
                return false;
        }
        if (!_isOpen) {
                printf ("SafeFile warning: averted write on unopened file\n");
                return false;
        }
        if (_fp == NULL) {
                printf ("SafeFile error: write FILE* is NULL\n");
        }

        int32_t writesize = fwrite (buffer, size, 1, _fp);

        if (writesize == -1 && !eof ()) {
                perror ("SafeFile error:");
        }
        return writesize;
}


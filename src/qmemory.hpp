/****************************************************************************

 Copyright (c) 2014, Hans Robeers
 All rights reserved.

 BSD 2-Clause License

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/

#ifndef QMEMORY_HPP
#define QMEMORY_HPP

#include <memory>
#include <QObject>

struct QObjectDeleter { void operator()(QObject* ptr) { ptr->deleteLater(); } };

template <typename T>
using qunique_ptr = std::unique_ptr<T, QObjectDeleter>;

template <typename _Tp1>
class qshared_ptr : public std::shared_ptr<_Tp1>
{
public:
    qshared_ptr() noexcept : std::shared_ptr<_Tp1>() {}
    qshared_ptr(_Tp1 *p) noexcept : std::shared_ptr<_Tp1>(p, QObjectDeleter()) {}

    void reset(_Tp1* p) noexcept
    { std::shared_ptr<_Tp1>(p, QObjectDeleter()).swap(*this); }
};

template <typename T>
qshared_ptr<T> make_qshared()
{
    return qshared_ptr<T>(new T());
}

template <typename T>
qshared_ptr<T> make_qshared(T* obj)
{
    return qshared_ptr<T>(obj);
}

#endif // QMEMORY_HPP

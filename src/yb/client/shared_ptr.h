// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef YB_CLIENT_SHARED_PTR_H
#define YB_CLIENT_SHARED_PTR_H

// YB uses c++11 features internally, but provides a client interface which
// does not require c++11. We use std::tr1::shared_ptr in our public interface
// to hold shared instances of YBClient, YBSession, and YBTable.
//
// Unfortunately, on OS X, libc++ is the default c++ standard library
// implementation and is required when compiling with c++11, but it does not
// include the tr1 APIs. As a workaround, we use std::shared_ptr on OS X, since
// OS X is for development only, and it is acceptable to require clients to
// compile with c++11.
//
// In order to allow applications to compile against YB on both Linux and OS
// X, we provide this typedef which resolves to std::tr1::shared_ptr on Linux
// and std::shared_ptr on OS X. Clients are encouraged to use these typedefs in
// order to ensure that applications will compile on both Linux and OS X.

// NOTE(akashnil): We are always compiling with C++11, so this in unnesseccary
// right now. The sp namespace will be renamed to std:: everywhere.

#include<memory>

namespace yb {
namespace client {
namespace sp {
  using std::shared_ptr;
  using std::weak_ptr;
  using std::enable_shared_from_this;
}
}
}

#endif // YB_CLIENT_SHARED_PTR_H

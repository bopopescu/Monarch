// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_LOADER_NACL_LISTENER_H_
#define COMPONENTS_NACL_LOADER_NACL_LISTENER_H_

#include <map>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "components/nacl/common/nacl_types.h"
#include "components/nacl/loader/nacl_trusted_listener.h"
#include "ipc/ipc_listener.h"

namespace IPC {
class AttachmentBrokerUnprivileged;
class SyncChannel;
class SyncMessageFilter;
}

// The NaClListener is an IPC channel listener that waits for a
// request to start a NaCl module.
class NaClListener : public IPC::Listener {
 public:
  NaClListener();
  ~NaClListener() override;
  // Listen for a request to launch a NaCl module.
  void Listen();

  bool Send(IPC::Message* msg);

#if defined(OS_LINUX)
  void set_prereserved_sandbox_size(size_t prereserved_sandbox_size) {
    prereserved_sandbox_size_ = prereserved_sandbox_size;
  }
#endif
#if defined(OS_POSIX)
  void set_number_of_cores(int number_of_cores) {
    number_of_cores_ = number_of_cores;
  }
#endif

  void* crash_info_shmem_memory() const { return crash_info_shmem_->memory(); }

  NaClTrustedListener* trusted_listener() const {
    return trusted_listener_.get();
  }

  typedef base::Callback<void(IPC::PlatformFileForTransit, base::FilePath)>
      ResolveFileTokenCallback;
  void ResolveFileToken(uint64_t token_lo,
                        uint64_t token_hi,
                        ResolveFileTokenCallback cb);
  void OnFileTokenResolved(uint64_t token_lo,
                           uint64_t token_hi,
                           IPC::PlatformFileForTransit ipc_fd,
                           base::FilePath file_path);

 private:
  bool OnMessageReceived(const IPC::Message& msg) override;

  typedef base::Callback<void(const IPC::Message&,
                              IPC::PlatformFileForTransit,
                              base::FilePath)> OpenResourceReplyCallback;

  bool OnOpenResource(const IPC::Message& msg,
                      const std::string& key,
                      OpenResourceReplyCallback cb);

  void OnAddPrefetchedResource(
      const nacl::NaClResourcePrefetchResult& prefetched_resource_file);
  void OnStart(const nacl::NaClStartParams& params);

  scoped_ptr<IPC::AttachmentBrokerUnprivileged> attachment_broker_;

  // A channel back to the browser.
  scoped_ptr<IPC::SyncChannel> channel_;

  // A filter that allows other threads to use the channel.
  scoped_refptr<IPC::SyncMessageFilter> filter_;

  base::WaitableEvent shutdown_event_;
  base::Thread io_thread_;

#if defined(OS_LINUX)
  size_t prereserved_sandbox_size_;
#endif
#if defined(OS_POSIX)
  // The outer sandbox on Linux and OSX prevents
  // sysconf(_SC_NPROCESSORS) from working; in Windows, there are no
  // problems with invoking GetSystemInfo.  Therefore, only in
  // OS_POSIX do we need to supply the number of cores into the
  // NaClChromeMainArgs object.
  int number_of_cores_;
#endif

  scoped_ptr<base::SharedMemory> crash_info_shmem_;

  scoped_refptr<NaClTrustedListener> trusted_listener_;

  ResolveFileTokenCallback resolved_cb_;

  // Used to identify what thread we're on.
  base::MessageLoop* main_loop_;

  typedef std::map<
    std::string,  // manifest key
    std::pair<IPC::PlatformFileForTransit,
              base::FilePath> > PrefetchedResourceFilesMap;
  PrefetchedResourceFilesMap prefetched_resource_files_;

  bool is_started_;

  DISALLOW_COPY_AND_ASSIGN(NaClListener);
};

#endif  // COMPONENTS_NACL_LOADER_NACL_LISTENER_H_

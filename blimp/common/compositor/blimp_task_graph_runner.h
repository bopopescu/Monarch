// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLIMP_COMMON_COMPOSITOR_BLIMP_TASK_GRAPH_RUNNER_H_
#define BLIMP_COMMON_COMPOSITOR_BLIMP_TASK_GRAPH_RUNNER_H_

#include "base/macros.h"
#include "base/threading/simple_thread.h"
#include "blimp/common/blimp_common_export.h"
#include "cc/raster/task_graph_runner.h"

namespace blimp {

// TaskGraphRunner that runs on a single thread.  See cc::TaskGraphRunner for
// details. Used by client's compositor and engine's dummy UI context factory.
class BLIMP_COMMON_EXPORT BlimpTaskGraphRunner
    : public cc::TaskGraphRunner,
      public base::DelegateSimpleThread::Delegate {
 public:
  BlimpTaskGraphRunner();
  ~BlimpTaskGraphRunner() override;

 private:
  // base::DelegateSimpleThread::Delegate implementation.
  void Run() override;

  base::DelegateSimpleThread worker_thread_;

  DISALLOW_COPY_AND_ASSIGN(BlimpTaskGraphRunner);
};

}  // namespace blimp

#endif  // BLIMP_COMMON_COMPOSITOR_BLIMP_TASK_GRAPH_RUNNER_H_

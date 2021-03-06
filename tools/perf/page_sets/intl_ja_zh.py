# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
from telemetry.page import page as page_module
from telemetry.page import shared_page_state
from telemetry import story


class IntlJaZhPage(page_module.Page):

  def __init__(self, url, page_set):
    super(IntlJaZhPage, self).__init__(
        url=url, page_set=page_set,
        shared_page_state_class=shared_page_state.SharedDesktopPageState)
    self.archive_data_file = 'data/intl_ja_zh.json'


class IntlJaZhPageSet(story.StorySet):

  """ Popular pages in Japanese and Chinese. """

  def __init__(self):
    super(IntlJaZhPageSet, self).__init__(
      archive_data_file='data/intl_ja_zh.json',
      cloud_storage_bucket=story.PARTNER_BUCKET)

    urls_list = [
      # Why: #5 Japanese site
      'http://www.amazon.co.jp',
      'http://mixi.jp/',
      'http://dtiblog.com/',
      'http://2ch.net/',
      'http://jugem.jp/',
      'http://hatena.ne.jp/',
      'http://goo.ne.jp/',
      # Why: #1 Japanese site
      'http://www.yahoo.co.jp/',
      # Why: #3 Japanese site
      'http://fc2.com/ja/',
      'http://kakaku.com/',
      'http://zol.com.cn/',
      'http://cn.yahoo.com/',
      # Why: #1 Chinese site
      'http://www.baidu.com/s?wd=%D0%C2%20%CE%C5',
      # Why: #2 Chinese site
      'http://www.qq.com/',
      # Why: #3 Chinese site
      'http://www.taobao.com/index_global.php',
      # Why: #4 Chinese site
      'http://www.sina.com.cn/',
      # Why: #5 Chinese site
      # pylint: disable=C0301
      'http://www.google.com.hk/#q=%E9%82%84%E6%8F%90%E4%BE%9B&fp=c44d333e710cb480',
      'http://udn.com/NEWS/mainpage.shtml',
      'http://ruten.com.tw/'
    ]

    for url in urls_list:
      self.AddStory(IntlJaZhPage(url, self))

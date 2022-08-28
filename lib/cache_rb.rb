# frozen_string_literal: true

require_relative "cache_rb/version"

require "extension"

module CacheRb
  class Error < StandardError; end

  def self.demo
    CacheRb::NativeHelpers.init

    _pid = Process.fork do
      CacheRb::NativeHelpers.insert(1, 1)
    end

    CacheRb::NativeHelpers.insert(2, 2)
    Process.wait

    CacheRb::NativeHelpers.display
  end
end

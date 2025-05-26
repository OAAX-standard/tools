// Copyright (c) OAAX. All rights reserved.
// Licensed under the Apache License, Version 2.0.

#include <stdio.h>

#include "sysinfo.h"  // NOLINT[build/include_subdir]

void test_basic_sysinfo() {
  SystemInfo info;
  get_system_info(&info);

  printf("CPU Name: %s\n", info.cpu_name);
  printf("CPU Cores: %d\n", info.cpu_cores);
  printf("Logical Processors: %d\n", info.logical_processors);
  printf("CPU Clock (MHz): %d\n", info.cpu_clock_mhz);
  printf("Hyperthreading Supported: %d\n", info.hyperthreading_supported);
  printf("Instruction Sets: %s\n", info.instruction_sets);
  printf("Total RAM (MB): %ld\n", info.total_ram_mb);
  printf("OS Name: %s\n", info.os_name);
  printf("Architecture: %s\n", info.architecture);
  printf("Is Virtual Machine: %d\n", info.is_virtual_machine);
}

void test_real_time_sysinfo() {
  RealTimeSystemInfo real_time_info;
  get_real_time_system_info(&real_time_info);

  printf("CPU Usage (%%): %.2f\n", real_time_info.cpu_usage_percent);
  printf("Used RAM (MB): %.ld\n", real_time_info.used_ram_mb);
  printf("RAM Usage (%%): %.2f\n", real_time_info.ram_usage_percent);
}

void test_sysinfo_main() {
  printf("Running basic system info test...\n");
  test_basic_sysinfo();

  printf("\nRunning real-time system info test...\n");
  test_real_time_sysinfo();

  printf("All tests completed successfully!\n");
}

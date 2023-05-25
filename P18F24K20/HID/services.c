/**
* This file is autogenerated by nRFgo Studio 1.17.1.3252
*/

#include "services.h"
#include "lib_aci.h"

static bool is_device_information_hardware_revision_string_set_requested = false;

static void *device_information_hardware_revision_string_set;
static uint8_t device_information_hardware_revision_string_set_size;

static bool device_information_hardware_revision_string_update_set(void)
{
  return(lib_aci_set_local_data(PIPE_DEVICE_INFORMATION_HARDWARE_REVISION_STRING_SET, (void*)device_information_hardware_revision_string_set, device_information_hardware_revision_string_set_size));
}

void services_set_device_information_hardware_revision_string(void *src, int size)
{
  device_information_hardware_revision_string_set_size = size;
  device_information_hardware_revision_string_set = src;
  is_device_information_hardware_revision_string_set_requested = true;
  services_update_pipes();
}

void services_update_pipes(void)
{
  if(is_device_information_hardware_revision_string_set_requested)
  {
    is_device_information_hardware_revision_string_set_requested = !(device_information_hardware_revision_string_update_set());
  }
}

#include "nvs_flash.h"
#include "nvs_component.h"

void nvs_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
}


uint8_t nvs_readKey(char *namespace_name, char* key, char* out_buffer, size_t *length)
{
    nvs_handle_t out_handle;
    if (nvs_open(namespace_name, NVS_READONLY, &out_handle) != ESP_OK) 
    {
        return NVS_ERROR_OPEN_NAMESPACE; 
    }
    nvs_get_str(out_handle, key, out_buffer, length);
    nvs_close(out_handle);
    return NVS_OK;
}

uint8_t nvs_writeKey(char *namespace_name, char* key, char* value)
{
    nvs_handle_t handle;
    if (nvs_open(namespace_name, NVS_READWRITE, &handle) != ESP_OK) 
    {
        return NVS_ERROR_OPEN_NAMESPACE; 
    }
    
    nvs_set_str(handle, key, value);
    nvs_commit(handle);
    nvs_close(handle);

    return NVS_OK;
}
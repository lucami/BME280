#ifndef NVS_COMPONENT_H
#define NVS_COMPONENT_H

#define NVS_OK                      0b0
#define NVS_ERROR_OPEN_NAMESPACE    0b1


void nvs_init();
uint8_t nvs_readKey(char *namespace_name, char* key, char* out_buffer, size_t *length);
uint8_t nvs_writeKey(char *namespace_name, char* key, char* value);


#endif // NVS_COMPONENT_H
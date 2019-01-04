/* ./md5.c */
void md5(const unsigned char *input, int len, unsigned char output[16]);
/* ./ubx.c */
const char *block_state_tostr(unsigned int state);
const char *get_typename(ubx_data_t *data);
int ubx_module_load(ubx_node_info_t *ni, const char *lib);
void ubx_module_unload(ubx_node_info_t *ni, const char *lib);
int ubx_node_init(ubx_node_info_t *ni, const char *name);
void ubx_node_cleanup(ubx_node_info_t *ni);
int ubx_block_register(ubx_node_info_t *ni, ubx_block_t *block);
ubx_block_t *ubx_block_get(ubx_node_info_t *ni, const char *name);
ubx_block_t *ubx_block_unregister(ubx_node_info_t *ni, const char *name);
int ubx_type_register(ubx_node_info_t *ni, ubx_type_t *type);
ubx_type_t *ubx_type_unregister(ubx_node_info_t *ni, const char *name);
ubx_type_t *ubx_type_get(ubx_node_info_t *ni, const char *name);
int ubx_resolve_types(ubx_block_t *b);
ubx_data_t *__ubx_data_alloc(const ubx_type_t *typ, const unsigned long array_len);
ubx_data_t *ubx_data_alloc(ubx_node_info_t *ni, const char *typname, unsigned long array_len);
int ubx_data_resize(ubx_data_t *d, unsigned int newlen);
void ubx_data_free(ubx_data_t *d);
int data_copy(ubx_data_t *dest, void *src, size_t size);
int ubx_data_copy(ubx_data_t *tgt, ubx_data_t *src);
unsigned int data_size(ubx_data_t *d);
int ubx_num_blocks(ubx_node_info_t *ni);
int ubx_num_types(ubx_node_info_t *ni);
int ubx_num_modules(ubx_node_info_t *ni);
void ubx_port_free_data(ubx_port_t *p);
int ubx_clone_port_data(ubx_port_t *p, const char *name, const char *doc, ubx_type_t *in_type, unsigned long in_data_len, ubx_type_t *out_type, unsigned long out_data_len, uint32_t state);
int ubx_config_assign(ubx_config_t *c, ubx_data_t *d);
void ubx_block_free(ubx_block_t *b);
ubx_block_t *ubx_block_create(ubx_node_info_t *ni, const char *type, const char *name);
int ubx_block_rm(ubx_node_info_t *ni, const char *name);
int ubx_port_connect_out(ubx_port_t *p, ubx_block_t *iblock);
int ubx_port_connect_in(ubx_port_t *p, ubx_block_t *iblock);
int ubx_ports_connect_uni(ubx_port_t *out_port, ubx_port_t *in_port, ubx_block_t *iblock);
int ubx_port_disconnect_out(ubx_port_t *out_port, ubx_block_t *iblock);
int ubx_port_disconnect_in(ubx_port_t *in_port, ubx_block_t *iblock);
int ubx_ports_disconnect_uni(ubx_port_t *out_port, ubx_port_t *in_port, ubx_block_t *iblock);
unsigned int get_num_configs(ubx_block_t *b);
ubx_config_t *ubx_config_get(ubx_block_t *b, const char *name);
ubx_data_t *ubx_config_get_data(ubx_block_t *b, const char *name);
long int ubx_config_get_data_ptr(ubx_block_t *b, const char *name, void **ptr);
long int ubx_config_data_len(ubx_block_t *b, const char *cfg_name);
int ubx_config_add(ubx_block_t *b, const char *name, const char *meta, const char *type_name, const unsigned long len);
int ubx_config_rm(ubx_block_t *b, const char *name);
unsigned int get_num_ports(ubx_block_t *b);
int ubx_port_add(ubx_block_t *b, const char *name, const char *doc, const char *in_type_name, unsigned long in_data_len, const char *out_type_name, unsigned long out_data_len, uint32_t state);
int ubx_outport_add(ubx_block_t *b, const char *name, const char *doc, const char *out_type_name, unsigned long out_data_len);
int ubx_inport_add(ubx_block_t *b, const char *name, const char *doc, const char *in_type_name, unsigned long in_data_len);
int ubx_port_rm(ubx_block_t *b, const char *name);
ubx_port_t *ubx_port_get(ubx_block_t *b, const char *name);
int ubx_block_init(ubx_block_t *b);
int ubx_block_start(ubx_block_t *b);
int ubx_block_stop(ubx_block_t *b);
int ubx_block_cleanup(ubx_block_t *b);
int ubx_cblock_step(ubx_block_t *b);
uint32_t __port_read(ubx_port_t *port, ubx_data_t *data);
void __port_write(ubx_port_t *port, ubx_data_t *data);
int ubx_clock_mono_gettime(struct ubx_timespec *uts);
int ubx_clock_mono_nanosleep(struct ubx_timespec *uts);
int ubx_ts_cmp(struct ubx_timespec *ts1, struct ubx_timespec *ts2);
void ubx_ts_norm(struct ubx_timespec *ts);
void ubx_ts_sub(struct ubx_timespec *ts1, struct ubx_timespec *ts2, struct ubx_timespec *out);
void ubx_ts_add(struct ubx_timespec *ts1, struct ubx_timespec *ts2, struct ubx_timespec *out);
void ubx_ts_div(struct ubx_timespec *ts, long div, struct ubx_timespec *out);
double ubx_ts_to_double(struct ubx_timespec *ts);
uint64_t ubx_ts_to_ns(struct ubx_timespec *ts);
const char *ubx_version(void);
int checktype(ubx_node_info_t *ni, ubx_type_t *required, const char *tcheck_str, const char *portname, int isrd);
long int cfg_getptr_char(ubx_block_t *b, const char *cfg_name, const char **valptr);
long int cfg_getptr_int(ubx_block_t *b, const char *cfg_name, const int **valptr);
long int cfg_getptr_uint(ubx_block_t *b, const char *cfg_name, const unsigned int **valptr);
long int cfg_getptr_uint8(ubx_block_t *b, const char *cfg_name, const uint8_t **valptr);
long int cfg_getptr_uint16(ubx_block_t *b, const char *cfg_name, const uint16_t **valptr);
long int cfg_getptr_uint32(ubx_block_t *b, const char *cfg_name, const uint32_t **valptr);
long int cfg_getptr_uint64(ubx_block_t *b, const char *cfg_name, const uint64_t **valptr);
long int cfg_getptr_int8(ubx_block_t *b, const char *cfg_name, const int8_t **valptr);
long int cfg_getptr_int16(ubx_block_t *b, const char *cfg_name, const int16_t **valptr);
long int cfg_getptr_int32(ubx_block_t *b, const char *cfg_name, const int32_t **valptr);
long int cfg_getptr_int64(ubx_block_t *b, const char *cfg_name, const int64_t **valptr);
long int cfg_getptr_float(ubx_block_t *b, const char *cfg_name, const float **valptr);
long int cfg_getptr_double(ubx_block_t *b, const char *cfg_name, const double **valptr);

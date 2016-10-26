int create_object_from_config(Config config, Object **result)
{
	if (!config_is_valid(config))
		return invalid_config;

	*result = new Object();

	int init_result = (**result).init_from_config(config);

	if (init_result != ok)
	{
		delete *result;
		return init_result;
	}

	return ok;
}

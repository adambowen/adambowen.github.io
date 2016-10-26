std::unique_ptr<Object> create_object_from_config_esc(Config config)
{
	if (!config_is_valid(config))
		throw invalid_config_error();

	std::unique_ptr<Object> result(new Object());
	result->init_from_config(config);
	return result;

}

int create_object_from_config(Config config, Object **result)
{
	try
	{
		std::unique_ptr<Object> temp = create_object_from_config_esc(config);
		// --- the critical line ---
		*result = temp.release();
		return ok;
	}
	catch(const invalid_config_error&)
	{
		return invalid_config;
	}
	catch(const other_error& e)
	{
		return e.code();
	}
	catch(...)
	{
		return unknown_error;
	}
}


-- A very simple arithmetic UDF with no arguments or return value.
-- It adds 10 to the value in bin 'age' and updates it.
function bin_age_add_10(rec)
	rec['age'] = rec['age'] + 10
	aerospike:update(rec)
end

-- A simple arithmetic UDF that has arguments and a return value.
-- It updates the value in the specified bin after performing the arithmetic
-- operation, and returns the resulting bin value.
function bin_transform(rec, bin_name, multiply_by_val, increment_by_val)
	rec[bin_name] = (rec[bin_name] * multiply_by_val) + increment_by_val
	aerospike:update(rec)
	return rec[bin_name]
end


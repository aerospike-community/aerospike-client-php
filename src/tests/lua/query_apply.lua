function update_salary(rec, offset)
    rec['salary'] = rec['salary'] + offset
    aerospike:update(rec)
end

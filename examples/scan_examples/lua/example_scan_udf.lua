function bin_transform(rec, offset)
    rec['age'] = rec['age'] + offset
    aerospike:update(rec)
end

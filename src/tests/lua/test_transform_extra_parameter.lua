function mytransform(rec, rec1, offset)
    rec['age'] = rec['age'] + offset
    aerospike:update(rec)
end

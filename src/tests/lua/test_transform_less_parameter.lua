function mytransform(rec, offset)
    --rec['age'] = rec['age'] + offset
    rec['age'] = rec['age'] + 2
    aerospike:update(rec)
end

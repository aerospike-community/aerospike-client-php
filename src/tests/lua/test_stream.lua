local function having_ge_threshold(bin_having, ge_threshold)
    return function(rec)
        if rec[bin_having] < ge_threshold then
            return false
        end
        return true
    end
end

local function count(group_by_bin)
  return function(group, rec)
    if rec[group_by_bin] then
      local bin_name = rec[group_by_bin]
      group[bin_name] = (group[bin_name] or 0) + 1
    end
    return group
  end
end

local function add_values(val1, val2)
  return val1 + val2
end

local function reduce_groups(a, b)
  return map.merge(a, b, add_values)
end

function group_count(stream, group_by_bin, bin_having, ge_threshold)
  if bin_having and ge_threshold then
    local myfilter = having_ge_threshold(bin_having, ge_threshold)
    return stream : filter(myfilter) : aggregate(map{}, count(group_by_bin)) : reduce(reduce_groups)
  else
    return stream : aggregate(map{}, count(group_by_bin)) : reduce(reduce_groups)
  end
end

local function names_filter(rec)
    if rec['first_name'] and rec['age'] then
        return true
    end
    return false
end

local function mapp_name_age(rec)
    return map{FirstName = rec.first_name, Age = rec.age}
end

function test_aggregate(s)
    return s : filter(names_filter) : map(mapp_name_age)
end

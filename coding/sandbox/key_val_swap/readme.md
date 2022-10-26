# Key value swap in std::map

The swapper application implements swapping of the keys and values in the std::map.
Functions take std::map and generate the std::map or std::multimap with values as keys.

## `is_swappable_to_map`

Function checks the uniqueness of values to decide, whether map or multimap
shall be used for an output.

## `swap_keys_with_values`

Swaps the keys with values, returns std::map.

## `swap_keys_with_multivalues`

Swaps the keys with values, returns std::multimap.

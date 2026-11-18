def run_pipeline(data, *stages):
    for stage in stages:
        data=stage(data)
    return data
def remove_negatives(numbers):
    return [num for num in numbers if num>=0]

def scale_by_factor(numbers, factor):
    return [num*factor for num in numbers]

def filter_even(numbers):
    return [num for num in numbers if num%2==0]

data=[10, -5, 3, 8, -12, 7, 4]
result=run_pipeline(data, remove_negatives, lambda numbers: scale_by_factor(numbers, 3), filter_even)
print(result)
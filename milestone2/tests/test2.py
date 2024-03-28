# this case tests inheritance

class country:
  def __init__(self, name: str, population: int, area: float):
    self.name: str = name
    self.population: int = population
    self.area: float = area

  def population_density(self) -> float:
    return self.population / self.area

  def is_larger_than(self, other: country) -> bool:
    return self.area > other.area
    

class developed_country(country):
  def __init__(self, name: str, population: int, area: float, index: float):
    country.__init__(self, name, population, area)
    self.index: float = index

  def quality_of_life(self) -> str:
    if self.index >= 0.8:
      return "Very High"
    elif 0.7 <= self.index < 0.8:
      return "High"
    elif 0.5 <= self.index < 0.7:
      return "Medium"
    else:
      return "Low"
    
    
class developing_country(country):
  def __init__(self, name: str, population: int, area: float, progress: int):
    country.__init__(self, name, population, area)
    self.progress: int = progress

  def growth_rate(self) -> str:
    if self.progress > 50:
      return "Excellent"
    elif 10 <= self.progress > 20:
      return "Good"
    elif 20 <= self.progress > 10:
      return "Fair"
    else:
      return "Poor"
    

def main() -> None:
  abc: developed_country = developed_country("abc", 2000, 900.0, 0.9)
  xyz: developing_country = developing_country("xyz", 1000, 300.0, 60)

  print("Population density of abc:", abc.population_density())
  print("progress of xyz:", xyz.progress)

if __name__ == "__main__":
  main()



#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle21_2020_Types
{
	struct Dish
	{
		vector<string> Ingredients;
		set<string> Allergens;
	};
}

using namespace Puzzle21_2020_Types;

static vector<Dish> ReadFoods(istream& input)
{
	regex dishFormat(R"(([^\(]+)\(contains ([^\)]+)\))");

	vector<Dish> foods;
	for (const string& line : ReadAllLines(input))
	{
		smatch match;
		if (regex_match(line, match, dishFormat))
		{
			Dish d;

			d.Ingredients = MakeEnumerator(StringSplit(match[1].str(), ' '))
				->Select<string>([](const string& s) { return Trim(s); })
				->ToVector();
			sort(d.Ingredients.begin(), d.Ingredients.end());

			d.Allergens = MakeEnumerator(StringSplit(match[2].str(), ','))
				->Select<string>([](const string& s) { return Trim(s); })
				->ToSet();

			foods.emplace_back(move(d));
		}
	}
	return foods;
}

static void Puzzle21_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Dish> dishes = ReadFoods(input);

	// Pre-processing;
	set<string> allIngredients;
	set<string> allAllergens;
	map<string, set<string>> allergenCandidateIngredients;
	for (const Dish& dish : dishes)
	{
		allIngredients.insert(dish.Ingredients.begin(), dish.Ingredients.end());
		allAllergens.insert(dish.Allergens.begin(), dish.Allergens.end());

		for (const string& allergenListed : dish.Allergens)
		{
			allergenCandidateIngredients[allergenListed].insert(dish.Ingredients.begin(), dish.Ingredients.end());
		}
	}

	// Filter down candidate ingredients to those commonly listed ingredients
	for (const string& allergen : allAllergens)
	{
		for (const Dish& dish : dishes)
		{
			if (dish.Allergens.contains(allergen) == false)
			{
				continue;
			}

			set<string>& candidates = allergenCandidateIngredients.at(allergen);

			set<string> filteredCandidates;
			set_intersection(candidates.begin(), candidates.end(),
				dish.Ingredients.begin(), dish.Ingredients.end(),
				inserter(filteredCandidates, filteredCandidates.begin()));

			allergenCandidateIngredients[allergen] = move(filteredCandidates);
		}
	}

	set<string> clearIngredients = allIngredients;
	for (map<string, set<string>>::const_reference allergen : allergenCandidateIngredients)
	{
		for (const string& ingredient : allergen.second)
		{
			clearIngredients.erase(ingredient);
		}
	}

	int64_t answer = 0;
	for (const Dish& dish : dishes)
	{
		answer += count_if(dish.Ingredients.begin(), dish.Ingredients.end(),
			[&clearIngredients](const string& ingredient)
			{
				return clearIngredients.contains(ingredient);
			});
	}

	printf("[2020] Puzzle21_A: %" PRId64 "\n", answer);
}

static void Puzzle21_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Dish> dishes = ReadFoods(input);

	// Pre-processing;
	set<string> allIngredients;
	set<string> allAllergens;
	map<string, set<string>> allergenCandidateIngredients;
	for (const Dish& dish : dishes)
	{
		allIngredients.insert(dish.Ingredients.begin(), dish.Ingredients.end());
		allAllergens.insert(dish.Allergens.begin(), dish.Allergens.end());

		for (const string& allergenListed : dish.Allergens)
		{
			allergenCandidateIngredients[allergenListed].insert(dish.Ingredients.begin(), dish.Ingredients.end());
		}
	}

	// Filter down candidate ingredients to those commonly listed ingredients
	for (const string& allergen : allAllergens)
	{
		for (const Dish& dish : dishes)
		{
			if (dish.Allergens.contains(allergen) == false)
			{
				continue;
			}

			set<string>& candidates = allergenCandidateIngredients.at(allergen);

			set<string> filteredCandidates;
			set_intersection(candidates.begin(), candidates.end(),
				dish.Ingredients.begin(), dish.Ingredients.end(),
				inserter(filteredCandidates, filteredCandidates.begin()));

			allergenCandidateIngredients[allergen] = move(filteredCandidates);
		}
	}

	map<string, string> allergenToIngredient;
	while (allergenToIngredient.size() != allAllergens.size())
	{
		map<string, set<string>>::const_iterator knownAllergen =
			find_if(allergenCandidateIngredients.begin(), allergenCandidateIngredients.end(),
				[](map<string, set<string>>::const_reference mapping)
				{
					return mapping.second.size() == 1;
				});
		assert(knownAllergen != allergenCandidateIngredients.end());

		string ingredientWithAllergen = *knownAllergen->second.begin();
		allergenToIngredient[knownAllergen->first] = ingredientWithAllergen;

		for (map<string, set<string>>::reference candidates : allergenCandidateIngredients)
		{
			candidates.second.erase(ingredientWithAllergen);
		}
	}

	string answer;
	for (map<string, string>::const_reference allergenPair : allergenToIngredient)
	{
		if (answer.empty() == false)
		{
			answer += ",";
		}
		answer += allergenPair.second;
	}

	printf("[2020] Puzzle21_B: %s\n", answer.c_str());
}

void Puzzle21_2020(const string& filename)
{
	Puzzle21_A(filename);
	Puzzle21_B(filename);
}

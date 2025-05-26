#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle15_2015_Types
{
	struct Ingredient
	{
		string Name;
		vector<int64_t> Properties;
	};

	struct Recipe
	{
		vector<Ingredient> Ingredients;
	};
}

using namespace Puzzle15_2015_Types;

static Recipe ParseRecipe(istream& input)
{
	Recipe r;
	for (const string& line : ReadEachLine(input))
	{
		Ingredient ingredient;
		ingredient.Properties.resize(5);

		char name[64] = { 0 };
		int scanned = sscanf(line.c_str(), "%[A-Za-z]: capacity %" SCNd64 ", durability %" SCNd64 ", flavor %" SCNd64 ", texture %" SCNd64 ", calories %" SCNd64,
			name,
			&ingredient.Properties[0],
			&ingredient.Properties[1],
			&ingredient.Properties[2],
			&ingredient.Properties[3],
			&ingredient.Properties[4]);
		assert(scanned == 6);
		(void)scanned;
		ingredient.Name = name;

		r.Ingredients.push_back(ingredient);
	}
	return r;
}

static int64_t ScoreAmountsWithoutCalories(const Recipe& r, const vector<int64_t>& amounts)
{
	vector<int64_t> propertyScores(r.Ingredients[0].Properties.size());

	for (size_t ingredientIndex = 0; ingredientIndex < r.Ingredients.size(); ingredientIndex++)
	{
		const Ingredient ingredient = r.Ingredients[ingredientIndex];
		for (size_t propertyIndex = 0; propertyIndex < ingredient.Properties.size(); propertyIndex++)
		{
			propertyScores[propertyIndex] += amounts[ingredientIndex] * ingredient.Properties[propertyIndex];
		}
	}

	// Exclude calories
	propertyScores.back() = 1;

	auto clampedScores = propertyScores | views::transform([](int64_t score) { return max(score, 0ll); });
	return accumulate(clampedScores.begin(), clampedScores.end(), 1ll, multiplies{});
}

static void FindHighestScoreWithoutCalories(const Recipe& r, size_t ingredientIndex, int64_t teaspoonsLeft, vector<int64_t> *amounts, int64_t* highestScore)
{
	if (ingredientIndex == (amounts->size() - 1))
	{
		(*amounts)[ingredientIndex] = teaspoonsLeft;
		int64_t score = ScoreAmountsWithoutCalories(r, *amounts);
		*highestScore = max(score, *highestScore);
		return;
	}

	for (int64_t spoons = 0; spoons <= teaspoonsLeft; spoons++)
	{
		(*amounts)[ingredientIndex] = spoons;
		FindHighestScoreWithoutCalories(r, ingredientIndex + 1, teaspoonsLeft - spoons, amounts, highestScore);
	}
}

static int64_t CountCalories(const Recipe& r, const vector<int64_t>& amounts)
{
	int64_t calories = 0;
	for (size_t ingredientIndex = 0; ingredientIndex < r.Ingredients.size(); ingredientIndex++)
	{
		const Ingredient ingredient = r.Ingredients[ingredientIndex];
		calories += ingredient.Properties.back() * amounts[ingredientIndex];
	}
	return calories;
}

static void FindHighestScoreWitSpecifiedCalories(const Recipe& r, int64_t calories, size_t ingredientIndex, int64_t teaspoonsLeft, vector<int64_t>* amounts, int64_t* highestScore)
{
	if (ingredientIndex == (amounts->size() - 1))
	{
		(*amounts)[ingredientIndex] = teaspoonsLeft;
		if (CountCalories(r, *amounts) == calories)
		{
			int64_t score = ScoreAmountsWithoutCalories(r, *amounts);
			*highestScore = max(score, *highestScore);
		}
		return;
	}

	for (int64_t spoons = 0; spoons <= teaspoonsLeft; spoons++)
	{
		(*amounts)[ingredientIndex] = spoons;
		FindHighestScoreWitSpecifiedCalories(r, calories, ingredientIndex + 1, teaspoonsLeft - spoons, amounts, highestScore);
	}
}

static void Puzzle15_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t teaspoons = 100;

	Recipe recipe = ParseRecipe(input);

	vector<int64_t> amounts(recipe.Ingredients.size());

	int64_t answer = 0;
	FindHighestScoreWithoutCalories(recipe, 0, teaspoons, &amounts, &answer);

	printf("[2015] Puzzle15_A: %" PRId64 "\n", answer);
}


static void Puzzle15_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t teaspoons = 100;
	const int64_t calories = 500;

	Recipe recipe = ParseRecipe(input);

	vector<int64_t> amounts(recipe.Ingredients.size());

	int64_t answer = 0;
	FindHighestScoreWitSpecifiedCalories(recipe, calories, 0, teaspoons, &amounts, &answer);

	printf("[2015] Puzzle15_B: %" PRId64 "\n", answer);
}

void Puzzle15_2015(const string& filename)
{
	Puzzle15_A(filename);
	Puzzle15_B(filename);
}

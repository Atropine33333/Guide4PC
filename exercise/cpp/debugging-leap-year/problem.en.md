The Leap Year Statute

Article 1 (Purpose). To unify the conversion between year numbers and the calendar, and to prevent February from gaining or losing a day, this statute is enacted.

Article 2 (Every fourth year). A year number divisible by 4 is a leap year.

Article 3 (No leap at the century). A year number divisible by 100, though it satisfies Article 2, is not a leap year.

Article 4 (Leap again at the fourth century). A year number divisible by 400, though it satisfies Article 3, is still a leap year.

Article 5 (Order of application). Apply Articles 2, 3 and 4 in that order; a later article prevails over an earlier one. The year 1900 satisfies Article 2 and fails Article 4, and is therefore common; the year 2000 satisfies Article 4 and is therefore leap.

Article 6 (Input). The year is one integer `year`, `1 <= year <= 9999`.

Article 7 (Output). Print `leap` for a leap year and `common` otherwise, in lowercase, with no spaces and no punctuation.

Article 8 (Supplementary). The input and output format stays as it is; only the decision logic in `is_leap` may be modified, and `main` shall not be touched.

**In plain words**: read one year and decide it by the leap-year rule: divisible by 4 and not by 100 is a leap year; divisible by 400 is also a leap year. Print `leap` or `common`. The starter's `is_leap` tests only `year % 4 == 0`; add the century and four-century clauses.

Sample input:

```text
1900
```

Sample output:

```text
common
```

Do not change the input/output format; only fix the decision logic.

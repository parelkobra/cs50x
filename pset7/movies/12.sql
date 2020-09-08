/* In 12.sql, write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.

Your query should output a table with a single column for the title of each movie.
You may assume that there is only one person in the database with the name Johnny Depp.
You may assume that there is only one person in the database with the name Helena Bonham Carter. */

SELECT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Johnny Depp'
AND stars.movie_id IN 
(
	SELECT stars.movie_id FROM stars
	JOIN movies ON movies.id = stars.movie_id
	JOIN people ON people.id = stars.person_id
	WHERE people.name = "Helena Bonham Carter"
)
ORDER BY movies.title

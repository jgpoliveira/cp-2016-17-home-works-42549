package cp.articlerep;

import cp.articlerep.ds.*;

import java.util.HashSet;

/**
 * @author Ricardo Dias
 */
public class Repository implements cp.articlerep.tentativas.Repository {

    private Map<String, List<Article>> byAuthor;
    private Map<String, List<Article>> byKeyword;
    private Map<Integer, Article> byArticleId;

    private int byIdInserts;
    private int byIdRemoves;

    private int byAuthorInserts;
    private int byAuthorRemoves;

    private int byKeywordInserts;
    private int byKeywordRemoves;

    public Repository(int nkeys) {
        this.byAuthor = new HashTable<String, List<Article>>(40000);
        this.byKeyword = new HashTable<String, List<Article>>(40000);
        this.byArticleId = new HashTable<Integer, Article>(40000);

        this.byIdInserts = 0;
        this.byIdRemoves = 0;

        this.byAuthorInserts = 0;
        this.byAuthorRemoves = 0;

        this.byKeywordInserts = 0;
        this.byKeywordRemoves = 0;
    }

    public boolean insertArticle(Article a) {
        /**
         *  BY ID INSERTION
         */
            if (byArticleId.contains(a.getId()))
                return false;

            byArticleId.put(a.getId(), a);
            byIdInserts++;

            /**
             *  BY AUTHOR INSERTION
             */
            Iterator<String> authors = a.getAuthors().iterator();
            while (authors.hasNext()) {
                String name = authors.next();

                List<Article> ll = byAuthor.get(name);

                if (ll == null) {
                    ll = new LinkedList<Article>();
                    byAuthor.put(name, ll);
                }
                ll.add(a);
                byAuthorInserts++;
            }

            /**
             *  BY KEYWORD INSERTION
             */
            Iterator<String> keywords = a.getKeywords().iterator();

            while (keywords.hasNext()) {
                String keyword = keywords.next();

                List<Article> ll = byKeyword.get(keyword);

                if (ll == null) {
                    ll = new LinkedList<Article>();
                    byKeyword.put(keyword, ll);
                }
                ll.add(a);
                byKeywordInserts++;
            }

            return true;
    }

    public void removeArticle(int id) {

        Article a;
        a = byArticleId.get(id);

        if (a == null)
            return;


            /**
             * BY KEYWORD REMOVE
             */
            Iterator<String> keywords = a.getKeywords().iterator();

            while (keywords.hasNext()) {
                String keyword = keywords.next();

                List<Article> ll = byKeyword.get(keyword);

                if (ll != null) {
                    int pos = 0;
                    Iterator<Article> it = ll.iterator();
                    while (it.hasNext()) {
                        Article toRem = it.next();
                        if (toRem == a) {
                            break;
                        }
                        pos++;
                    }

                    ll.remove(pos);
                    it = ll.iterator();
                    if (!it.hasNext()) { // checks if the list is empty
                        byKeyword.remove(keyword);
                    }
                }
                byKeywordRemoves++;
            }


            /**
             * BY AUTHOR REMOVE
             */
            Iterator<String> authors = a.getAuthors().iterator();

            while (authors.hasNext()) {
                String name = authors.next();

                List<Article> ll = byAuthor.get(name);
                if (ll != null) {
                    int pos = 0;
                    Iterator<Article> it = ll.iterator();
                    while (it.hasNext()) {
                        Article toRem = it.next();
                        if (toRem == a) {
                            break;
                        }
                        pos++;
                    }

                    ll.remove(pos);
                    it = ll.iterator();
                    if (!it.hasNext()) { // checks if the list is empty
                        byAuthor.remove(name);
                    }
                }
                byAuthorRemoves++;
            }

            /**
             * BY ID REMOVE
             */
            byArticleId.remove(id);
            byIdRemoves++;
    }

    public List<Article> findArticleByAuthor(List<String> authors) {

            List<Article> res = new LinkedList<Article>();
            Iterator<String> it = authors.iterator();

            while (it.hasNext()) {
                String name = it.next();

                List<Article> as = byAuthor.get(name);

                if (as != null) {
                    Iterator<Article> ait = as.iterator();
                    while (ait.hasNext()) {
                        Article a = ait.next();
                        res.add(a);
                    }
                }
            }

            return res;

    }

    public List<Article> findArticleByKeyword(List<String> keywords) {

            List<Article> res = new LinkedList<Article>();
            Iterator<String> it = keywords.iterator();

            while (it.hasNext()) {
                String keyword = it.next();

                List<Article> as = byKeyword.get(keyword);
                if (as != null) {
                    Iterator<Article> ait = as.iterator();
                    while (ait.hasNext()) {
                        Article a = ait.next();
                        res.add(a);
                    }
                }
            }

            return res;
    }


    /**
     * This method is supposed to be executed with no concurrent thread
     * accessing the repository.
     *
     */
    public boolean validate() {

        HashSet<Integer> articleIds = new HashSet<Integer>();
        int articleCount = 0;
        int authorCount = 0;
        int keywordCount = 0;

        Iterator<Article> aIt = byArticleId.values();
        while(aIt.hasNext()) {
            Article a = aIt.next();

            articleIds.add(a.getId());
            articleCount++;

            // check the authors consistency
            Iterator<String> authIt = a.getAuthors().iterator();
            while(authIt.hasNext()) {
                String name = authIt.next();
                if (!searchAuthorArticle(a, name)) {
                    return false;
                }
                authorCount++;
            }

            // check the keywords consistency
            Iterator<String> keyIt = a.getKeywords().iterator();
            while(keyIt.hasNext()) {
                String keyword = keyIt.next();
                if (!searchKeywordArticle(a, keyword)) {
                    return false;
                }
                keywordCount++;
            }
        }

        Iterator<List<Article>> itAuthor = byAuthor.values();
        int mapAuthorCount = 0;
        while(itAuthor.hasNext()) {
            List<Article> cur = itAuthor.next();
            mapAuthorCount += cur.size();
        }

        Iterator<List<Article>> itKeyword = byKeyword.values();
        int mapKeywordCount = 0;
        while(itKeyword.hasNext()) {
            List<Article> cur = itKeyword.next();
            mapKeywordCount += cur.size();
        }

        System.out.println("ARTICLE COUNT: " + articleCount + ", " + articleIds.size());
        System.out.println("AUTHOR COUNT: " + authorCount + ", " + mapAuthorCount);
        System.out.println("KEYWORD COUNT: " + keywordCount + ", " + mapKeywordCount);

        System.out.println("ID COUNT: " + articleCount + ", inserts ["+ byIdInserts +"] - removes ["+ byIdRemoves +"] = " + (byIdInserts - byIdRemoves));
        System.out.println("AUTHOR COUNT: " + mapAuthorCount + ", inserts ["+ byAuthorInserts +"] - removes ["+ byAuthorRemoves+"] = " + (byAuthorInserts - byAuthorRemoves));
        System.out.println("KEYWORD COUNT: " + mapKeywordCount + ", inserts ["+ byKeywordInserts +"] - removes ["+ byKeywordRemoves+"] = " + (byKeywordInserts - byKeywordRemoves));

        return articleCount == articleIds.size() &&
                authorCount == mapAuthorCount &&
                keywordCount == mapKeywordCount &&
                articleCount == byIdInserts - byIdRemoves &&
                mapAuthorCount == byAuthorInserts - byAuthorRemoves &&
                mapKeywordCount == byKeywordInserts - byKeywordRemoves;
    }

    private boolean searchAuthorArticle(Article a, String author) {
        List<Article> ll = byAuthor.get(author);
        if (ll != null) {
            Iterator<Article> it = ll.iterator();
            while (it.hasNext()) {
                if (it.next() == a) {
                    return true;
                }
            }
        }
        return false;
    }

    private boolean searchKeywordArticle(Article a, String keyword) {
        List<Article> ll = byKeyword.get(keyword);
        if (ll != null) {
            Iterator<Article> it = ll.iterator();
            while (it.hasNext()) {
                if (it.next() == a) {
                    return true;
                }
            }
        }
        return false;
    }

}
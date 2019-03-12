#!/usr/bin/env python3

import os, sys
import urllib.parse
import copy
import json


def enc_print(string='', encoding='utf8'):
    sys.stdout.buffer.write(string.encode(encoding) + b'\n')


enc_print('''Content-type: text/html\n
<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1,
      shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet"
      href="/static/bootstrap.min.css"
      integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T"
      crossorigin="anonymous">

    <title>Hello, world!</title>
  </head>
  <body>
    <nav class="navbar navbar-expand-lg navbar-light bg-light shadow-sm">
      <div class="container">
        <a class="navbar-brand" href="/">Library</a>
        <button class="navbar-toggler" type="button"
          data-toggle="collapse" data-target="#navbarSupportedContent"
          aria-controls="navbarSupportedContent" aria-expanded="false"
          aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse"
          id="navbarSupportedContent">
          <ul class="navbar-nav mr-auto">
            <li class="nav-item">
              <a class="nav-link" href="/persons">Persons</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/authors">Authors</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/students">Students</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/teachers">Teachers</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/librarians">Librarians</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/groups">Groups</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/publishers">Publishers</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/student_cards">Student cards</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/teacher_cards">Teacher cards</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/books">Books</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="/book_author">Books and authors</a>
            </li>
          </ul>
        </div>
      </div>
    </nav>
    <div class="container">''')


html = {
  'head': '<h1 class="mt-2">Hello, world!</h1>',
  'error': "",
  'content': "",
}

try:
  import mysql.connector

  def create_table(name, thead, data):
    head = '<h1 class="mt-2">%s <a href="/%s/edit/0">+</a></h1>'% \
              (name.capitalize(), name)
    content = '''<table class="table"><thead><tr>'''
    content += '<th scope="col">#</th>'
    for i in thead:
      content += '<th scope="col">%s</th>' % i
    content += '<th scope="col">Handle</th></tr></thead><tbody>'
    for i in data:
      content += \
        '<tr><th scope="row"><a href="/%s/edit/%s">%s</td></th>' \
        % (name, i[0], i[0])
      for j in range(len(thead)):
        content += '<td><a href="/%s/edit/%s">%s</td>'\
                      % (name, i[0], i[j + 1])
      content += '<td><a href="/%s/delete/%s">-</a></td></tr>'\
              % (name, i[0])
    content += '</tbody></table>'
    return head, content

  def create_input_text(name_id, name, data):
    return '''
<div class="form-group row">
  <label for="{0}" class="col-sm-2 col-form-label">{1}</label>
  <div class="col-sm-10">
    <input type="name" class="form-control" name="{0}" id="{0}" placeholder="{1}" value="{2}">
  </div>
</div>'''.format(name_id, name, data)

  def create_error(message):
    return "<h1 class='mt-2 text-danger'>%s</h1>" % message

  def create_alert(name, message):
    return '<div class="alert alert-%s" role="alert">%s</div>' \
      % (name, message)

  class DataColumn:
    def __init__(self, name, type = 0,
                 link_table = "", link_column = "id"):
      self.name = name
      self.type = type
      self.link_table = link_table
      self.link_column = link_column

    def __str__(self):
      return self.name

    __repr__ = __str__

  class ViewColumn:
    def __init__(self, name, pattern, column_ids):
      self.name = name
      self.pattern = pattern
      self.column_ids = column_ids

    def __str__(self):
      return self.name

    __repr__ = __str__

  class ViewColumnLink:
    def __init__(self, name, column):
      self.name = name
      self.column = column

    def __str__(self):
      return self.name

    __repr__ = __str__

  class Table:
    def __init__(self, name, data, view,
        main_view = ViewColumn("", "%s", [1])):
      self.name = name
      self.data = data
      self.view = view
      self.main_view = main_view

  class Query:
    def __init__(
      self, database, table, parent_table = -1,
      parent_column = "", link_column = "id"
    ):
      self.database = database
      self.tables = [(
        table, link_column, parent_table, parent_column
      )]
      self.data = []
      self.data_dict = {}
      self.view = []
      self.generated = 0

    def append_data(self, column, table = 0):
      data_set = (table, column)
      if data_set not in self.data_dict:
        self.data.append((
          data_set, self.tables[table][0].data[column]
        ))
        self.data_dict[data_set] = len(self.data) - 1

    def append_view(self, column, table = 0):
      self.view.append((table, column))

    def get_data_column(self, column, table = 0):
      return self.data[self.get_data_column_id(column, table)][1]

    def get_data_column_id(self, column, table = 0):
      return self.data_dict[(table, column)]

    def merge(self, query):
      tables_size = len(self.tables)
      self.tables += query.tables
      for i in query.data:
        self.append_data(i[0][1], i[0][0] + tables_size)
      for i in query.view:
        self.append_view(i[1], i[0] + tables_size)

    def collect_view(
      self, main = False, full = False, recursive = True,
      new_main_name = False, current_table = 0
    ):
      table = self.tables[0][0]
      data = table.data
      views = table.view
      if not main or full:
        self.append_data(0)
      if main:
        views = [table.main_view]
      for k, v in enumerate(views):
        if type(v) is ViewColumn:
          if main and new_main_name:
            self.append_view(ViewColumn(
              new_main_name, v.pattern, v.column_ids
            ))
          else:
            self.append_view(v)
          for j in v.column_ids:
            self.append_data(j)
        elif type(v) is ViewColumnLink \
            and data[v.column].link_table in self.database.tables:
          if recursive:
            if data[v.column].type == 1:
              t = self.database.tables[data[v.column].link_table]
              q = Query(
                self.database, t, current_table, data[v.column].name,
                data[v.column].link_column
              )
              q.collect_view(
                True, full, True, v.name, current_table + len(self.tables)
              )
              self.merge(q)
            elif data[v.column].type == 2:
              t = self.database.tables[data[v.column].link_table]
              q = Query(
                self.database, t, current_table, "id",
                data[v.column].link_column
              )
              q.collect_view(
                True, full, True, v.name, current_table + len(self.tables)
              )
              self.merge(q)
          else:
            self.append_view(v)
            self.append_data(v.column)
      self.generated = 1

    def generate_select(self, recursive = True):
      if not self.generated:
        self.collect_view(recursive = recursive)
      return "SELECT %s FROM `%s` AS `0` %s" % (
        ", ".join(
          "`%s`.`%s`" % (
            i[0][0], i[1].name
          ) for i in self.data
        ), self.tables[0][0].name, self.generate_joins()
      )

    def generate_insert(self, data):
      cols = [i.name for i in self.tables[0][0].data]
      for k, v in data.items():
        if k not in cols:
          return ""
      k, v = zip(*(("`%s`" % k, "`%s`" % v) for k, v in data.items()))
      return "INSERT INTO %s (%s) VALUES (%s)" % (
        self.tables[0][0].name,  ", ".join(k), ", ".join(v)
      )

    def generate_update(self, data, data_id):
      if not self.generated:
        self.collect_view(False, False, False)
      return "UPDATE %s SET %s WHERE id = %s" % (
        self.table_name,  ", ".join(
          "`%s`" % i[1].name
            for k, v in self.data[1:]
              if k[0] == 0 and v[1].name != "id"
        )
      )

    def generate_joins(self):
      out = []
      for k, v in enumerate(self.tables[:]):
        if v[2] != -1:
          s = "INNER JOIN `%s` `%s` ON `%s`.`%s` = `%s`.`%s`" % (
            v[0].name, k, v[2], v[3], k, v[1]
          )
          out.append(s)
      return  " ".join(out)



  class Database:
    def __init__(self, db):
      self.db = db
      self.tables = {
        "persons": Table("persons", [
          DataColumn("id"),
          DataColumn("last_name"),
          DataColumn("first_name"),
          DataColumn("middle_name"),
        ], [
          ViewColumn("Last name", "%s", [1]),
          ViewColumn("First name", "%s", [2]),
          ViewColumn("Middle name", "%s", [3]),
        ], ViewColumn("Person name", "%s %s %s", [1, 2, 3])),
        "authors": Table("authors", [
          DataColumn("id"),
          DataColumn("person_id", 1, "persons"),
        ], [
          ViewColumnLink("Author name", 1),
        ], ViewColumnLink("Person name", 1)),
        "students": Table("students", [
          DataColumn("id"),
          DataColumn("person_id", 1, "persons"),
          DataColumn("group_id", 1, "groups"),
        ], [
          ViewColumnLink("Student name", 1),
          ViewColumnLink("Group name", 2),
        ], ViewColumnLink("Person name", 1)),
        "teachers": Table("teachers", [
          DataColumn("id"),
          DataColumn("person_id", 1, "persons"),
        ], [
          ViewColumnLink("Teacher name", 1),
        ], ViewColumnLink("Person name", 1)),
        "librarians": Table("librarians", [
          DataColumn("id"),
          DataColumn("person_id", 1, "persons"),
        ], [
          ViewColumnLink("Librarian name", 1),
        ], ViewColumnLink("Person name", 1)),
        "languages": Table("languages", [
          DataColumn("id"),
          DataColumn("name"),
        ], [
          ViewColumn("Name", "%s", [1]),
        ]),
        "groups": Table("groups", [
          DataColumn("id"),
          DataColumn("name"),
        ], [
          ViewColumn("Name", "%s", [1]),
        ]),
        "publishers": Table("publishers", [
          DataColumn("id"),
          DataColumn("name"),
        ], [
          ViewColumn("Name", "%s", [1]),
        ]),
        "student_cards": Table("student_cards", [
          DataColumn("id"),
          DataColumn("student_id", 1, "students"),
          DataColumn("librarian_id", 1, "librarians"),
          DataColumn("take_date"),
          DataColumn("return_date"),
        ], [
          ViewColumnLink("Student name", 1),
          ViewColumnLink("Librarian name", 2),
          ViewColumn("Take date", "%s", [3]),
          ViewColumn("Return date", "%s", [4]),
        ]),
        "teacher_cards": Table("teacher_cards", [
          DataColumn("id"),
          DataColumn("teacher_id", 1, "teachers"),
          DataColumn("librarian_id", 1, "librarians"),
          DataColumn("take_date"),
          DataColumn("return_date"),
        ], [
          ViewColumnLink("Teachers name", 1),
          ViewColumnLink("Librarian name", 2),
          ViewColumn("Take date", "%s", [3]),
          ViewColumn("Return date", "%s", [4]),
        ]),
        "books": Table("books", [
          DataColumn("id"),
          DataColumn("name"),
          DataColumn("year"),
          DataColumn("isbn"),
          DataColumn("publisher_id", 1, "publishers"),
          DataColumn("language_id", 1, "languages"),
          DataColumn("authors", 2, "book_author", "book_id"),
        ], [
          ViewColumn("Name", "%s", [1]),
          ViewColumn("Year", "%s", [2]),
          ViewColumn("ISBN", "%s", [3]),
          ViewColumnLink("Publisher", 4),
          ViewColumnLink("Language", 5),
          ViewColumnLink("Author", 6),
        ]),
        "book_author": Table("book_author", [
          DataColumn("id"),
          DataColumn("book_id", 1, "books"),
          DataColumn("author_id", 1, "authors"),
        ], [
          ViewColumnLink("Book name", 1),
          ViewColumnLink("Author name", 2),
        ], ViewColumnLink("Author name", 2)),
      }

    def get_list(self, table_name):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = Query(self, self.tables[table_name])
      q = query.generate_select()
      #enc_print(q)
      cursor = self.db.cursor()
      cursor.execute(q)
      result = cursor.fetchall()
      names = []
      for i in query.view:
        names.append(i[1].name)
      data = [
        [i[0]] + [
          (j[1].pattern % tuple(i[query.data_dict[(j[0], k)]]
            for k in j[1].column_ids))
              for j in query.view
        ] for i in result
      ]
      return (names, data)

    def get_edit(self, table_name, id, error = ""):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = Query(self, self.tables[table_name])
      query.collect_view(recursive = False)

      views = query.view
      data = ["" for i in range(len(views) + 1)]
      page_name = table_name[:-1]

      if id != 0:
        q = query.generate_select(False) + "WHERE `0`.id = %s" % (id)
        cursor = self.db.cursor()
        cursor.execute(q)
        result = cursor.fetchall()
        if len(result) == 0:
          error += create_alert("danger","No such " + page_name)
        else: data = result[0]
      form = ""
      for k, v in enumerate(views):
        if type(v[1]) is ViewColumn:
          if v[1].pattern == "%s":
            col_id = v[1].column_ids[0]
            col = query.get_data_column(col_id)
            form += create_input_text(
              col, v[1].name, data[query.get_data_column_id(col_id)]
            )
        elif type(v[1]) is ViewColumnLink:
            col_id = v[1].column
            col = query.get_data_column(col_id)
            form += create_input_text(
              col, v[1].name, data[query.get_data_column_id(col_id)]
            )

      head = '<h1 class="mt-2">Edit %s</h1>' % page_name
      content = '''
<form>
  <input type="hidden" name="id" value="%s">
  %s
  <div class="form-group row">
    <div class="col-sm-10">
      <button type="submit" class="btn btn-primary">Edit</button>
      <a class="btn btn-primary" href="/%s/delete/%s">Delete</a>
    </div>
  </div>
</form>
''' % (id, form, table_name, id)
      return head, error, content

    def create_or_edit(self, table_name, parsed_query):
      data_id = int(parsed_query['id'][0])
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = Query(self, self.tables[table_name])
      query.collect_view(recursive = False)

      views = query.view
      #for i in query.data[1:]:
      #  if i[1].name not in parsed_query:
      #    return self.get_edit(
      #      table_name, 0,
      #      create_alert("danger", "Failed to create new")
      #    )
      q = ""
      if data_id == 0:
        data = {
          i[1].name: parsed_query[i[1].name][0]
            for i in query.data[1:]
        }
        q = query.generate_insert(data)
      else:
        data = ["%s = '%s'" % (i[1].name, parsed_query[i[1].name][0])
                for i in query.data_columns[1:]]
        q = "UPDATE %s SET %s WHERE id = %s" % \
            (table_name, ", ".join(data), data_id)
      try:
        cursor = self.db.cursor()
        cursor.execute(q)
        if data_id == 0:
          if cursor.lastrowid:
            return self.get_edit(table_name, cursor.lastrowid,
                create_alert("success", "Done"))
          else:
            return self.get_edit(table_name, 0,
                create_alert("danger", "Failed to create or edit"))
        else:
          return self.get_edit(table_name, data_id,
              create_alert("success", "Done"))
      except mysql.connector.Error as error:
        return self.get_edit(table_name, 0,
            create_alert("danger", error))
      return ("","","")

    def delete(self, table_name, id):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = "DELETE FROM %s WHERE id = %s" % (table_name, id)
      try:
        cursor = self.db.cursor()
        cursor.execute(query)
        cursor.close()
        return create_alert("success", "Done")
      except mysql.connector.Error as error:
        return create_alert("danger", error)



  class App:
    def __init__(self):
      self.db = mysql.connector.connect(
        host="localhost",
        database="library",
        user="library",
        passwd="V9KaCHDIGSzAHyzj"
      )

      url = os.environ["REQUEST_URI"]
      parsed_url = urllib.parse.urlparse(url)
      self.parsed_query = urllib.parse.parse_qs(parsed_url.query)
      self.path = parsed_url.path.split('/')
      self.table = self.path[1]
      self.operation = ""
      self.selected_id = 0
      if len(self.path) > 2:
        self.operation = self.path[2]
      if len(self.path) > 3:
        self.selected_id = int(self.path[3])
      self.database = Database(self.db)

      global html

      if self.table in self.database.tables:
        if self.operation == "edit":
          if 'id' in self.parsed_query:
            html['head'], html['error'], html['content'] = \
                self.database.create_or_edit(self.table, self.parsed_query)
          else:
            html['head'], html['error'], html['content'] = \
                self.database.get_edit(self.table, self.selected_id)
        elif self.operation == "delete":
          html['error'] = self.database.delete(self.table, self.selected_id)
          try:
            names, result = self.database.get_list(self.table)
            html['head'], html['content'] = \
              create_table(self.table, names, result)
          except:
            pass
        else:
          names, result = self.database.get_list(self.table)
          html['head'], html['content'] = \
              create_table(self.table, names, result)
      elif self.table == 'books':
        cursor.execute("SELECT books.id, books.name, year, publishers.name, languages.name FROM books LEFT JOIN publishers ON publisher_id = publishers.id LEFT JOIN languages ON language_id = languages.id")
        result = cursor.fetchall()
        books = dict((r[0], list(r + ("",))) for r in result)
        cursor.execute("SELECT books.id, last_name, first_name, middle_name FROM books LEFT JOIN book_author ON books.id = book_author.book_id LEFT JOIN authors ON book_author.author_id = authors.id LEFT JOIN persons ON authors.person_id = persons.id")
        authors = cursor.fetchall()
        for a in authors:
          b = books[a[0]]
          if b[5] != "": b[5] += "<br>"
          b[5] += "%s %s %s" % (a[1], a[2], a[3])
        html = '%s' % books
        html = create_table("books",
          ("Name","Year","Publisher","Language","Author"),
          list(books.values()))

    def __del__(self):
      self.db.commit()


  app = App()

except Exception as e:
  import traceback
  html['head'] = "<h1 class='mt-2 text-danger'>%s</h1>" % (e,)
  html['content'] = '<pre>%s<pre>' % str(traceback.format_exc())
except:
  html = "Unexpected error:", sys.exc_info()[0]

enc_print('''%s%s%s
    </div>

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="/static/jquery-3.3.1.slim.min.js"
    integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
    crossorigin="anonymous"></script>
    <script src="/static/popper.min.js"
    integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1"
    crossorigin="anonymous"></script>
    <script src="/static/bootstrap.min.js"
    integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM"
    crossorigin="anonymous"></script>
  </body>
</html>''' % (html['head'], html['error'], html['content']))

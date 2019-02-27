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
          </ul>
        </div>
      </div>
    </nav>
    <div class="container">''')


html = '''<h1 class="mt-2">Hello, world!</h1>'''



try:
  import mysql.connector

  def create_table(name, thead, data):
    html = '''<h1 class="mt-2">%s <a href="/%s/edit/0">+</a></h1>
    <table class="table"><thead><tr>''' % (name.capitalize(), name)
    html += '<th scope="col">#</th>'
    for i in thead:
      html += '<th scope="col">%s</th>' % i
    html += '<th scope="col">Handle</th></tr></thead><tbody>'
    for i in data:
      html += \
        '<tr><th scope="row"><a href="/%s/edit/%s">%s</td></th>' \
        % (name, i[0], i[0])
      for j in range(len(thead)):
        html += '<td><a href="/%s/edit/%s">%s</td>'\
                      % (name, i[0], i[j + 1])
      html += '<td><a href="/%s/delete/%s">-</a></td></tr>'\
              % (name, i[0])
    html += '</tbody></table>'
    return html

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

  def create_alert(message):
    return '<div class="alert alert-danger" role="alert">%s</div>' \
      % message

  class DataColumn:
    def __init__(self, name, type = 0,
                 link_table = ""):
      self.name = name
      self.type = type
      self.link_table = link_table

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

  class Table:
    def __init__(self, name, data, view,
        main_view = [ViewColumn("", "%s", [1])]):
      self.name = name
      self.data = data
      self.view = view
      self.main_view = main_view

  class Query:
    def __init__(self, table_name, parent_column = ""):
      self.table_name = table_name
      self.tables = [[[table_name], parent_column]]
      self.data_columns = []
      self.view_columns = []

    def append_data(self, column, table = 0):
      self.data_columns.append((table, copy.copy(column)))

    def append_view(self, column):
      inc = len(self.data_columns)
      s = len(column.column_ids)
      self.view_columns.append(ViewColumn(column.name, column.pattern,
                       [i + inc for i in range(s)]))

    def insert(self, query):
      table_size = len(self.tables)
      data_size = len(self.tables)
      inc = len(self.data_columns)
      self.tables += [ [[self.table_name] + i[0], i[1]]
          for i in query.tables ]
      for i in query.data_columns:
        self.append_data(i[1], i[0] + table_size)

      column = query.view_columns[0]
      s = len(column.column_ids)
      self.view_columns[-1].pattern = column.pattern
      self.view_columns[-1].column_ids = \
          [i + inc for i in range(s)]

    def gen_main_query(self):
      return ", ".join("%s.%s" %
                     ("_".join(self.tables[i[0]][0]), i[1].name)
                     for i in self.data_columns) \
            + " FROM " + self.table_name

    def gen_dep_query(self):
      out = []
      for i in self.tables[1:]:
        path = "_".join(i[0][:])
        s = "LEFT JOIN %s %s ON %s.%s = %s.id" % \
            (i[0][-1], path, "_".join(i[0][:-1]), i[1], path)
        out.append(s)
      return  " ".join(out)



  class Database:
    def __init__(self, cursor):
      self.cursor = cursor
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
        ], [ViewColumn("Person name", "%s %s %s", [1, 2, 3])]),
        "authors": Table("authors", [
          DataColumn("id"),
          DataColumn("person_id", type = 1, link_table = "persons"),
        ], [
          ViewColumn("Author name", "%s", [1]),
        ]),
        "students": Table("students", [
          DataColumn("id"),
          DataColumn("person_id", type = 1, link_table = "persons"),
        ], [
          ViewColumn("Student name", "%s", [1]),
        ]),
        "teachers": Table("teachers", [
          DataColumn("id"),
          DataColumn("person_id", type = 1, link_table = "persons"),
        ], [
          ViewColumn("Teacher name", "%s", [1]),
        ]),
        "librarians": Table("librarians", [
          DataColumn("id"),
          DataColumn("person_id", type = 1, link_table = "persons"),
        ], [
          ViewColumn("Librarian name", "%s", [1]),
        ]),
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
          DataColumn("student_id", type = 1, link_table = "students"),
          DataColumn("librarian_id", type = 1, link_table = "librarians"),
          DataColumn("take_date"),
          DataColumn("return_date"),
        ], [
          ViewColumn("Student name", "%s", [1]),
          ViewColumn("Librarian name", "%s", [2]),
          ViewColumn("Take date", "%s", [3]),
          ViewColumn("Return date", "%s", [4]),
        ]),
        "teacher_cards": Table("teacher_cards", [
          DataColumn("id"),
          DataColumn("teacher_id", type = 1, link_table = "teachers"),
          DataColumn("librarian_id", type = 1, link_table = "librarians"),
          DataColumn("take_date"),
          DataColumn("return_date"),
        ], [
          ViewColumn("Teachers name", "%s", [1]),
          ViewColumn("Librarian name", "%s", [2]),
          ViewColumn("Take date", "%s", [3]),
          ViewColumn("Return date", "%s", [4]),
        ]),
        "books": Table("books", [
          DataColumn("id"),
          DataColumn("name"),
          DataColumn("year"),
          DataColumn("isbn"),
          DataColumn("publisher_id", type = 1, link_table = "publishers"),
          DataColumn("language_id", type = 1, link_table = "languages"),
          DataColumn("authors", type = 2, link_table = "book_author"),
        ], [
          ViewColumn("Name", "%s", [1]),
          ViewColumn("Year", "%s", [2]),
          ViewColumn("ISBN", "%s", [3]),
          ViewColumn("Publisher", "%s", [4]),
          ViewColumn("Language", "%s", [5]),
          #ViewColumn("Author", "%s", [3]),
        ]),
      }

    def gen_col_query(self, table_name, main = False, full = False, parent_column = "", recursive = True):
      if table_name not in self.tables:
        return False
      query = Query(table_name, parent_column)
      if not main or full:
        query.append_data(self.tables[table_name].data[0])

      table = self.tables[table_name]
      data = table.data
      views = table.view
      if main and not full:
        views = table.main_view[0:1]
      for i in views:
        query.append_view(i)
        for j in i.column_ids:
          data_col = data[j]
          if data_col.type == 0:
            query.append_data(data_col)
          elif data_col.type == 1:
            if recursive:
              query.insert(self.gen_col_query(data_col.link_table, True, full, data_col.name))
            else:
              query.append_data(data_col)
      return query

    def get_list(self, table_name):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = self.gen_col_query(table_name)
      q = "SELECT %s %s" % \
          (query.gen_main_query(), query.gen_dep_query())
      enc_print(q)
      self.cursor.execute(q)
      result = self.cursor.fetchall()
      names = []
      for i in query.view_columns:
        names.append(i.name)
      data = [[i[0]] + [(j.pattern % tuple(i[k] for k in j.column_ids)) for j in query.view_columns] for i in result]
      return (names, data)

    def get_edit(self, table_name, id):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = self.gen_col_query(table_name, recursive = False)
      views = query.view_columns
      data = ["" for i in range(len(views) + 1)]
      error = ""
      page_name = table_name[:-1]

      if id != 0:
        q = "SELECT %s %s WHERE %s.id = %s"\
              % (query.gen_main_query(), query.gen_dep_query(),
                 table_name, id)
        self.cursor.execute(q)
        result = self.cursor.fetchall()
        if len(result) == 0:
          error = create_alert("No such " + page_name)
        else: data = result[0]
      form = ""
      for k, v in enumerate(views):
        if v.pattern == "%s":
          col_id = v.column_ids[0]
          col = query.data_columns[v.column_ids[0]][1].name
          form += create_input_text(col, v.name, data[col_id]);
      html = '''
<h1 class="mt-2">Edit %s</h1>
%s
<form>
  <input type="hidden" name="id" value="%s">
  %s
  <div class="form-group row">
    <div class="col-sm-10">
      <button type="submit" class="btn btn-primary">Add</button>
    </div>
  </div>
</form>
''' % (page_name, error, id, form)
      return html


  class App:
    def __init__(self):
      db = mysql.connector.connect(
        host="localhost",
        database="library",
        user="library",
        passwd="V9KaCHDIGSzAHyzj"
      )
      self.cursor = db.cursor()

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
      self.database = Database(self.cursor)

      global html

      if self.table in self.database.tables:
        if self.operation == "edit":
          if 'id' in self.parsed_query:
            pass
          else:
            html = self.database.get_edit(self.table, self.selected_id)
        else:
          names, result = self.database.get_list(self.table)
          html = create_table(self.table, names, result)
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


  app = App()

except Exception as e:
  import traceback
  html = "<h1 class='mt-2 text-danger'>%s</h1>" % (e,)
  html += '<pre>%s<pre>' % str(traceback.format_exc())
except:
  html = "Unexpected error:", sys.exc_info()[0]

enc_print('''%s
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
</html>''' % html)

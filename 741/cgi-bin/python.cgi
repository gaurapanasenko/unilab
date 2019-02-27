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

    def gen_insert_query(self):
      return self.table_name + " (" + ", ".join(i[1].name
                     for i in self.data_columns[1:]) + ")"

    def gen_dep_query(self):
      out = []
      for i in self.tables[1:]:
        path = "_".join(i[0][:])
        s = "LEFT JOIN %s %s ON %s.%s = %s.id" % \
            (i[0][-1], path, "_".join(i[0][:-1]), i[1], path)
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
          DataColumn("group_id", type = 1, link_table = "groups"),
        ], [
          ViewColumn("Student name", "%s", [1]),
          ViewColumn("Group name", "%s", [2]),
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

    def gen_col_query(self, table_name, main = False, full = False,
        parent_column = "", recursive = True):
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
              query.insert(self.gen_col_query(data_col.link_table,
                True, full, data_col.name))
            else:
              query.append_data(data_col)
      return query

    def get_list(self, table_name):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = self.gen_col_query(table_name)
      q = "SELECT %s %s" % \
          (query.gen_main_query(), query.gen_dep_query())
      #enc_print(q)
      cursor = self.db.cursor()
      cursor.execute(q)
      result = cursor.fetchall()
      names = []
      for i in query.view_columns:
        names.append(i.name)
      data = [[i[0]] + [(j.pattern % tuple(i[k] for k in j.column_ids))
                        for j in query.view_columns] for i in result]
      return (names, data)

    def get_edit(self, table_name, id, error = ""):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = self.gen_col_query(table_name, recursive = False)
      views = query.view_columns
      data = ["" for i in range(len(views) + 1)]
      page_name = table_name[:-1]

      if id != 0:
        q = "SELECT %s %s WHERE %s.id = %s"\
              % (query.gen_main_query(), query.gen_dep_query(),
                 table_name, id)
        cursor = self.db.cursor()
        cursor.execute(q)
        result = cursor.fetchall()
        if len(result) == 0:
          error += create_alert("danger","No such " + page_name)
        else: data = result[0]
      form = ""
      for k, v in enumerate(views):
        if v.pattern == "%s":
          col_id = v.column_ids[0]
          col = query.data_columns[v.column_ids[0]][1].name
          form += create_input_text(col, v.name, data[col_id]);
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
      query = self.gen_col_query(table_name, recursive = False)
      views = query.view_columns
      for i in query.data_columns[1:]:
        if i[1].name not in parsed_query:
          return self.get_edit(table_name, 0,
              create_alert("danger", "Failed to create new"))
      q = ""
      if data_id == 0:
        data = ["'%s'" % parsed_query[i[1].name][0]
                for i in query.data_columns[1:]]
        q = "INSERT INTO %s VALUES (%s)" % \
            (query.gen_insert_query(), ", ".join(data))
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

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
      href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css"
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
              <a class="nav-link" href="/book_author">Book author</a>
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

  def make_singular(string):
    return string[:-1] if string[-1] == 's' else string

  def humanize(string):
    return string.replace("_", " ").replace("s ", " ")

  def create_table(name, thead, data):
    args = (humanize(name).capitalize(), name)
    head = '<h1 class="mt-2">%s <a href="/%s/edit/0">+</a></h1>' % args
    content = '<table class="table"><thead><tr>'
    content += '<th scope="col">#</th>'
    for i in thead:
      content += '<th scope="col">%s</th>' % i
    content += '<th scope="col"></th></tr></thead><tbody>'
    for i in data:
      s = '<tr><th scope="row"><a href="/%s/edit/%s">%s</td></th>'
      content += s % (name, i[0], i[0])
      for j in range(len(thead)):
        args = (name, i[0], i[j + 1])
        content += '<td><a href="/%s/edit/%s">%s</td>' % args
      h = '<td><a href="/%s/delete/%s" class="close">'
      h += '&times;</a></td></tr>'
      content += h % (name, i[0])
    content += '</tbody></table>'
    return head, content

  def create_input_text(name_id, name, data):
    return '''
<div class="form-group row">
  <label for="{0}" class="col-sm-2 col-form-label">{1}</label>
  <div class="col-sm-10">
    <input type="name" class="form-control" name="{0}"
      id="{0}" placeholder="{1}" value="{2}">
  </div>
</div>'''.format(name_id, name, data)

  def create_select(name_id, name, data_id, data):
    s = lambda i : "selected" if str(i[0]) == data_id else ""
    l = lambda i : (i[0], s(i), i[1])
    a = ["<option value='%s' %s>%s</option>" % l(i) for i in data]
    return '''
<div class="form-group row">
  <label for="{0}" class="col-sm-2 col-form-label">{1}</label>
  <div class="col-sm-10">
    <select class="form-control custom-select" name="{0}" id="{0}">
      <option value='0'>None</option>
      {2}
    </select>
  </div>
</div>'''.format(name_id, name, "".join(a))

  def create_error(message):
    return "<h1 class='mt-2 text-danger'>%s</h1>" % message

  def create_alert(name, message):
    args = (name, message)
    return '<div class="alert alert-%s" role="alert">%s</div>' % args

  ##############
  # DataColumn #
  ##############
  class DataColumn:
    def __init__(self, name, kind = 0, related_table = "",
    related_column = "id"):
      if not isinstance(name, str):
        raise TypeError("name argument must be string")
      if not isinstance(kind, int) or kind < 0 or kind > 2:
        raise TypeError("kind argument must be int from 0 to 2")
      if not isinstance(related_table, str):
        raise TypeError("related_table argument must be string")
      if not isinstance(related_column, str):
        raise TypeError("related_column argument must be string")
      self._name = name
      self._kind = kind
      self._related_table = related_table
      self._related_column = related_column

    def get_name(self):
      return self._name
    def get_kind(self):
      return self._kind
    def get_related_table(self):
      return self._related_table
    def get_related_column(self):
      return self._related_column
    __str__ = get_name
    __repr__ = get_name

  ##############
  # ViewColumn #
  ##############
  class ViewColumn:
    def __init__(self, name, pattern = "", data_columns = []):
      if not isinstance(name, str):
        raise TypeError("name argument must be string")
      if not isinstance(pattern, str):
        raise TypeError("pattern argument must be string")
      if not isinstance(data_columns, (list, tuple)):
        raise TypeError("data_columns argument must be list or tuple")
      for i in data_columns:
        if not isinstance(i, int):
          msg = "data_columns argument must contain only integers"
          raise TypeError(msg)
      self._name = name
      self._pattern = pattern
      self._data_columns = data_columns

    def get_name(self):
      return self._name
    def get_pattern(self):
      return self._pattern
    def get_data_columns(self):
      return self._data_columns
    def get_data_column(self, index = 0):
      return self._data_columns[index]
    __str__ = get_name
    __repr__ = get_name

  ##################
  # ViewColumnLink #
  ##################
  class ViewColumnLink:
    def __init__(self, name, column):
      if not isinstance(name, str):
        raise TypeError("name argument must be string")
      if not isinstance(column, int):
        raise TypeError("column argument must be int")
      self._name = name
      self._column = column

    def get_name(self):
      return self._name
    def get_pattern(self):
      return "%s"
    def get_data_columns(self):
      return [self._column]
    def get_data_column(self):
      return self._column
    __str__ = get_name
    __repr__ = get_name

  #########
  # Table #
  #########
  class Table:
    def __init__(self, name, data, view,
    main_view = ViewColumn("", "%s", [1])):
      if not isinstance(name, str):
        raise TypeError("name argument must be string")
      if not isinstance(data, (list, tuple)):
        raise TypeError("data argument must be list or tuple")
      for i in data:
        if not isinstance(i, DataColumn):
          msg = "data argument must contain only DataColumn"
          raise TypeError(msg)
      if not isinstance(view, (list, tuple)):
        raise TypeError("data argument must be list or tuple")
      for i in view:
        if not isinstance(i, (ViewColumn, ViewColumnLink)):
          msg = "view argument must contain ViewColumn or ViewColumnLink"
          raise TypeError(msg)
      if not isinstance(main_view, (ViewColumn, ViewColumnLink)):
        msg = "main_view argument must be ViewColumn or ViewColumnLink"
        raise TypeError(msg)
      self._name = name
      self._data = data
      self._view = view
      self._main_view = main_view

    def get_name(self):
      return self._name
    def get_singular(self):
      return make_singular(humanize(self._name))
    def get_data(self, index = False):
      if isinstance(index, int) and index is not False:
        return self._data[index]
      else:
        return self._data
    def get_view(self, index = False):
      if isinstance(index, int) and index is not False:
        return self._view[index]
      else:
        return self._view
    def get_main_view(self):
      return self._main_view

  class Query:
    def __init__(self, database, table, parent_table = -1,
    parent_column = "", link_column = "id"):
      self._database = database
      self._tables = [(table, link_column, parent_table, parent_column)]
      self._data_columns = []
      self._data_columns_dict = {}
      self._view_columns = []
      self._view_columns_dict = {}
      self._generated = 0
      self._query = False
      self._data = False
      self._fetchable = False

    def get_table(self, index = 0):
      return self._tables[index][0]

    def append_data(self, column, table = 0):
      if not isinstance(column, int):
        raise TypeError("column argument must be int")
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      data_set = (table, column)
      if data_set not in self._data_columns_dict:
        s = (data_set, self.get_table(table).get_data(column))
        self._data_columns.append(s)
        self._data_columns_dict[data_set] = len(self._data_columns) - 1

    def append_view(self, column, table = 0):
      if not isinstance(column, (ViewColumn, ViewColumnLink)):
        msg = "column argument must be ViewColumn or ViewColumnLink"
        raise TypeError(msg)
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      view_set = (table, column.get_name())
      if view_set not in self._view_columns_dict:
        self._view_columns.append((table, column))
        self._view_columns_dict[view_set] = len(self._view_columns) - 1

    def get_data_columns(self):
      return (i[1] for i in self._data_columns)

    def get_data_column(self, column, table = 0):
      if not isinstance(column, int):
        raise TypeError("column argument must be int")
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      i = self.get_data_column_id(column, table)
      return self._data_columns[i][1]

    def get_data_column_id(self, column, table = 0):
      if not isinstance(column, int):
        raise TypeError("column argument must be int")
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      return self._data_columns_dict[(table, column)]

    def get_data_column_id_by_name(self, column, table = 0):
      if not isinstance(column, str):
        raise TypeError("column argument must be string")
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      e = enumerate(self._data_columns)
      return next(k for k, v in e if v[1].get_name() == column)

    def get_view_columns(self):
      return (i[1] for i in self._view_columns)

    def get_view_column(self, column, table = 0):
      if not isinstance(column, str):
        raise TypeError("column argument must be string")
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      i = self.get_view_column_id(column, table)
      return self._view_columns[i][1]

    def get_view_column_id(self, column, table = 0):
      if not isinstance(column, str):
        raise TypeError("column argument must be string")
      if not isinstance(table, int):
        raise TypeError("table argument must be int")
      return self._view_columns_dict[(table, column)]

    def merge(self, query):
      if not isinstance(query, Query):
        raise TypeError("query argument must be Query")
      tables_size = len(self._tables)
      self._tables += query._tables
      for i in query._data_columns:
        self.append_data(i[0][1], i[0][0] + tables_size)
      for i in query._view_columns:
        self.append_view(i[1], i[0] + tables_size)

    def collect_view(self, main = False, with_id = True,
    recursive = True,new_main_name = False, current_table = 0):
      table = self._tables[0][0]
      data = table.get_data()
      views = table.get_view()
      if with_id:
        self.append_data(0)
      if main:
        views = [table.get_main_view()]
      for k, v in enumerate(views):
        if type(v) is ViewColumn:
          if main and new_main_name:
            args = [new_main_name, v.get_pattern(), v.get_data_columns()]
            self.append_view(ViewColumn(*args))
          else:
            self.append_view(v)
          for j in v.get_data_columns():
            self.append_data(j)
        elif type(v) is ViewColumnLink:
          rt = data[v.get_data_column()].get_related_table()
          if rt in self._database.tables:
            col = v.get_data_column()
            if recursive:
              if data[col].get_kind() in [1, 2]:
                t = self._database.tables[data[col].get_related_table()]
                r = data[col].get_name()
                if data[col].get_kind() == 2:
                  r = "id"
                rc = data[col].get_related_column()
                q = Query(self._database, t, current_table, r, rc)
                ct = current_table + len(self._tables)
                nmn = new_main_name if new_main_name else v.get_name()
                q.collect_view(True, False, True, nmn, ct)
                self.merge(q)
            else:
              if data[col].get_kind() == 1:
                self.append_view(v)
                self.append_data(col)
      self._generated = 1

    def generate_select(
      self, main = False, with_id = True, recursive = True
    ):
      if not self._generated:
        self.collect_view(main, with_id, recursive)
      self._query = "SELECT %s FROM `%s` AS `0` %s" % (
        ", ".join(
          "`%s`.`%s`" % (
            i[0][0], i[1].get_name()
          ) for i in self._data_columns
        ), self.get_table(0).get_name(), self.generate_joins()
      )
      self._fetchable = True
      return self

    def generate_insert(self, data):
      if not self._generated:
        self.collect_view(recursive = False)
      cols = [i.get_name() for i in self.get_table(0).get_data()]
      for k, v in data.items():
        if k not in cols:
          return self
      k, v = zip(*(("`%s`" % k, '"%s"' % v) for k, v in data.items()))
      args = (self.get_table(0).get_name(),  ", ".join(k), ", ".join(v))
      self._query = "INSERT INTO %s (%s) VALUES (%s)" % args
      return self

    def generate_update(self, data, data_id):
      if not self._generated:
        self.collect_view(recursive = False)
      cols = [i.get_name() for i in self.get_table(0).get_data()]
      for k, v in data.items():
        if k not in cols:
          return self
      self._query = "UPDATE `%s` SET %s WHERE `id` = \"%s\"" % (
        self.get_table().get_name(),  ", ".join(
          "`%s` = \"%s\"" % (k, v)
            for k, v in data.items()
        ), data_id
      )
      return self

    def generate_joins(self):
      out = []
      for k, v in enumerate(self._tables[:]):
        if v[2] != -1:
          args = (v[0].get_name(), k, v[2], v[3], k, v[1])
          s = "INNER JOIN `%s` `%s` ON `%s`.`%s` = `%s`.`%s`" % args
          out.append(s)
      return " ".join(out)

    def where(self, column, data, table = 0):
      if self._query:
        self._query += " WHERE `%s`.`%s` = \"%s\"" % (table, column, data)
      return self

    def execute(self):
      if self._query:
        cursor = self._database.db.cursor()
        enc_print("%s<br>\n" % self._query)
        cursor.execute(self._query)
        if self._fetchable:
          result = cursor.fetchall()
          self._data = []
          for i in result:
            r = [i[0]]
            for j in self._view_columns:
              s = []
              for k in j[1].get_data_columns():
                s.append(i[self._data_columns_dict[(j[0], k)]])
              r.append(j[1].get_pattern() % tuple(s))
            self._data.append(r)
          return self._data
        else:
          return cursor

    def get_column_names(self):
      return [i.get_name() for i in self.get_view_columns()]

    def get_data(self):
      return self._data



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
          DataColumn("book_id", 1, "books"),
          DataColumn("student_id", 1, "students"),
          DataColumn("librarian_id", 1, "librarians"),
          DataColumn("take_date"),
          DataColumn("return_date"),
        ], [
          ViewColumnLink("Book name", 1),
          ViewColumnLink("Student name", 2),
          ViewColumnLink("Librarian name", 3),
          ViewColumn("Take date", "%s", [4]),
          ViewColumn("Return date", "%s", [5]),
        ]),
        "teacher_cards": Table("teacher_cards", [
          DataColumn("id"),
          DataColumn("book_id", 1, "books"),
          DataColumn("teacher_id", 1, "teachers"),
          DataColumn("librarian_id", 1, "librarians"),
          DataColumn("take_date"),
          DataColumn("return_date"),
        ], [
          ViewColumnLink("Book name", 1),
          ViewColumnLink("Teachers name", 2),
          ViewColumnLink("Librarian name", 3),
          ViewColumn("Take date", "%s", [4]),
          ViewColumn("Return date", "%s", [5]),
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
      data = query.generate_select().execute()
      return (query.get_column_names(), data)

    def get_edit(self, table_name, id, error = "", new_data = {},
    create = False):
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = Query(self, self.tables[table_name])
      query.collect_view(recursive = False)

      views = list(query.get_view_columns())
      data = ["" for i in range(len(views) + 1)]
      for k, v in new_data.items():
        data[query.get_data_column_id_by_name(k)] = v
      page_name = table_name[:-1]

      if id != 0:
        result = query.generate_select(False).where("id", id).execute()
        if len(result) == 0:
          error += create_alert("danger","No such " + page_name)
        else: data = result[0]
      form = ""
      for k, v in enumerate(views):
        if type(v) is ViewColumn:
          if v.get_pattern() == "%s":
            col_id = v.get_data_column()
            col = query.get_data_column(col_id)
            dt_cid = data[query.get_data_column_id(col_id)]
            form += create_input_text(col, v.get_name(), dt_cid)
        elif type(v) is ViewColumnLink:
            col_id = v.get_data_column()
            col = query.get_data_column(col_id)
            if col.get_kind() == 1:
              q = Query(self, self.tables[col.get_related_table()])
              dt = q.generate_select(True, True, True).execute()
              dt_cid = data[query.get_data_column_id(col_id)]
              form += create_select(col, v.get_name(), dt_cid, dt)
      coe = "Create" if create else "Edit"
      head = '<h1 class="mt-2">%s %s</h1>' % (coe, humanize(page_name))
      btns = '''
      <button type="submit" class="btn btn-primary">Edit</button>
      <a class="btn btn-primary" href="/%s/delete/%s">
      Delete</a>''' % (table_name, id)
      if create:
        btns = '''<button type="submit"
        class="btn btn-primary">Create</button>'''
      content = '''
<form>
  <input type="hidden" name="id" value="%s">
  %s
  <div class="form-group row">
    <div class="col-sm-10">%s</div>
  </div>
</form>
''' % (id, form, btns)
      return head, error, content

    def create_or_edit(self, table_name, parsed_query):
      data_id = int(parsed_query['id'][0])
      if table_name not in self.tables:
        return create_error("No such table " % table_name)
      query = Query(self, self.tables[table_name])
      query.collect_view(recursive = False)

      views = query.get_view_columns()
      #for i in query.data[1:]:
      #  if i[1].name not in parsed_query:
      #    return self.get_edit(
      #      table_name, 0,
      #      create_alert("danger", "Failed to create new")
      #    )
      data = {
        i.get_name(): parsed_query[i.get_name()][0]
          for i in list(query.get_data_columns())[1:]
            if i.get_name() in parsed_query
      }
      if data_id == 0:
        query.generate_insert(data)
      else:
        query.generate_update(data, data_id)
      try:
        cursor = query.execute()
        if data_id == 0:
          if cursor.lastrowid:
            alrt = create_alert("success", "Done")
            return self.get_edit(table_name, cursor.lastrowid, alrt)
          else:
            alrt = create_alert("danger", "Failed to create or edit")
            return self.get_edit(table_name, 0, alrt, data)
        else:
          return self.get_edit(table_name, data_id,
              create_alert("success", "Done"))
      except mysql.connector.Error as error:
        errorMessage = error
        if error.errno == mysql.connector.errorcode.ER_DUP_ENTRY:
          name = str(query.get_table().get_singular())
          errorMessage = "Same %s already exists" % name
        alert = create_alert("danger", errorMessage)
        return self.get_edit(table_name, 0, alert, data)
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
        message = error
        if error.errno == mysql.connector.errorcode.ER_ROW_IS_REFERENCED_2:
          name = str(make_singular(table_name))
          arr = str(error).split("`")
          table = humanize(arr[3]).capitalize()
          message = "This %s is used in table '%s'" % (name, table)
        return create_alert("danger", message)



  class App:
    def __init__(self):
      mysql_data = {
        "host": "localhost",
        "database": "library",
        "user": "library",
        "passwd": "V9KaCHDIGSzAHyzj"
      }
      self.db = mysql.connector.connect(**mysql_data)

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
            args = (self.table, self.parsed_query)
            h = self.database.create_or_edit(*args)
            html['head'], html['error'], html['content'] = h
          else:
            c = self.selected_id == 0
            h = self.database.get_edit(self.table, self.selected_id, create = c)
            html['head'], html['error'], html['content'] = h
        elif self.operation == "delete":
          he = self.database.delete(self.table, self.selected_id)
          html['error'] = he
          try:
            names, result = self.database.get_list(self.table)
            t = create_table(self.table, names, result)
            html['head'], html['content'] = t
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
    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js"
    integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
    crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"
    integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1"
    crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"
    integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM"
    crossorigin="anonymous"></script>
  </body>
</html>''' % (html['head'], html['error'], html['content']))

#include <iostream>
#include <mysql/mysql.h>


void select_and_show_data(MYSQL *mysql)
{
    MYSQL_RES *res;  /// MYSQL_RES 此结构表示返回行的查询结果 (SELECT、SHOW、DESCRIBE、EXPLAIN)。
    MYSQL_ROW row;  ///  MYSQL_ROW 一行数据 类型安全
    //sql_cmd = "DESCRIBE people;";
    std::string sql_cmd = "SELECT * FROM people";
    /// mysql_real_query 执行指定字符串长度的SQL查询。
    // int flag = mysql_real_query (&mysql, sql_cmd.c_str(), sql_cmd.length());
    int flag = mysql_query (mysql, sql_cmd.c_str()); // 也可以使用 mysql_query
    if (flag)
    {
        printf("Error %u: %s\n", mysql_errno(mysql), mysql_error(mysql));
        exit(1);
    }
    else
    {
        printf ("query success !\n");

        /// mysql_store_result 检索完整的结果集
        res = mysql_store_result (mysql); // 获取结果集

        /// mysql_field_count 返回最后一个指令中结果的列的数量
        unsigned int num_columns  = mysql_field_count(mysql); // 结果集中 列数
        unsigned int num_rows = res->row_count; // 结果集中 行数

        printf ("num_columns = %d\n", num_columns);
        printf ("num_rows = %d\n", num_rows);

        /// mysql_fetch_row 取得结果集的下一行（注：返回行结构体）
        row = mysql_fetch_row(res); // 检索结果集的下一行 每行有 num_columns 列
        std::string result;
        int index = 0;
        while (row)
        {
            printf ("%d\t", ++index);

            for (int t = 0; t < num_columns; t++)
                printf ("%s\t", row[t]);
            printf ("\n");
            row = mysql_fetch_row(res);
        }
        printf ("index = %d\n", index);
    }
    /// mysql_free_result 释放结果集使用的内存
    mysql_free_result(res);
}


/// Mysql c api 操作  增 删 改 查
int main() {
    std::cout << "Hello, World!" << std::endl;
    MYSQL mysql;   /// MYSQL 表示一个数据库连接的处理程序
    MYSQL_RES *res;  /// MYSQL_RES 此结构表示返回行的查询结果 (SELECT、SHOW、DESCRIBE、EXPLAIN)。
    MYSQL_ROW row;  ///  MYSQL_ROW 一行数据 类型安全

    /// 初始化一个mysql的结构体
    mysql_init (&mysql); /// mysql_init 创建或初始化一个mysql的结构体

    char optvalue = 6;
    {/// 参数设置
        /// https://dev.mysql.com/doc/c-api/8.0/en/mysql-options.html
        /// mysql_options 设置连接建立函数(如MYSQL_REAL_CONNECT())的连接选项
        mysql_options(&mysql, MYSQL_OPT_CONNECT_TIMEOUT, (char*)&optvalue);
        optvalue = 1;
        mysql_options(&mysql, MYSQL_OPT_RECONNECT, (char*)&optvalue);
        optvalue = 2;
        mysql_options(&mysql, MYSQL_OPT_READ_TIMEOUT, (char*)&optvalue);
    }


    {/// 连接 mysql
        /// mysql_real_connect 与一个MYSQL的服务器连接
        void *ret = mysql_real_connect(&mysql, "localhost", "root", "leacock", nullptr, 0, nullptr, 0);
        if (ret == nullptr) // 失败为 null ，成功为 MySQL*连接处理程序，也就是 第一个参数 &mysql
        {
            /// mysql_errno 返回最后使用的mysql函数的错误码
            /// mysql_error 返回最后使用的mysql函数的错误消息
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            return 0;
        }
    }

    printf ("Connected MySQL successfully!\n");

    std::string sql_cmd;

    {/// 删除 数据库A 如果存在A
        sql_cmd = "DROP DATABASE IF EXISTS testdb";
        /// mysql_query 执行一个用以空结尾的字符串指定的SQL指令
        if (mysql_query(&mysql, sql_cmd.c_str())) {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }
    }


    {/// 新建 库
        sql_cmd = "CREATE DATABASE testdb  DEFAULT charset=utf8";
        if (mysql_query(&mysql, sql_cmd.c_str())) {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }
    }


    {/// 选择数据库
        sql_cmd = "USE testdb";
        if (mysql_query(&mysql, sql_cmd.c_str())) {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }
    }


    {/// 新建 表
        sql_cmd = "CREATE TABLE people(id INT primary key not null,"
                  "age int default null, "
                  "name VARCHAR(25) default null)";
        if (mysql_query(&mysql, sql_cmd.c_str())) {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }
    }



    {/// 向表中插入数据
        mysql_query(&mysql, "INSERT INTO people(id,age,name) values('3',25,'张三')");
        mysql_query(&mysql, "INSERT INTO people(id,age,name) values('4',25,'李四')");
    }



    {/// select 检索数据 与 遍历展示
        select_and_show_data(&mysql);
    }


    {/// 事务处理 回滚 例子
        sql_cmd = "BEGIN;";
        mysql_query (&mysql, sql_cmd.c_str());
        sql_cmd = "DELETE FROM people WHERE id=4";
        int flag = mysql_query (&mysql, sql_cmd.c_str());
        if (flag)
        {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }

        flag = mysql_rollback(&mysql); /// mysql_rollback 回滚事务
        if (flag)
        {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }

        select_and_show_data(&mysql);
        /// 最后结果还是有 id = 4 的数据
    }

    {/// 事务处理 提交 例子
        sql_cmd = "BEGIN;";
        mysql_query (&mysql, sql_cmd.c_str());
        sql_cmd = "DELETE FROM people WHERE id=4";
        int flag = mysql_query (&mysql, sql_cmd.c_str());
        if (flag)
        {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }

        flag = mysql_commit(&mysql); /// mysql_commit 提交事务
        if (flag)
        {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }
        flag = mysql_rollback(&mysql); /// mysql_rollback 回滚事务
        if (flag)
        {
            printf("Error %u: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
            exit(1);
        }

        select_and_show_data(&mysql);
        /// 最后结果没有 id = 4 的数据， 已经提交
    }

    /// mysql_close 关闭与MySQL的服务器连接
    mysql_close(&mysql);

    return 0;
}

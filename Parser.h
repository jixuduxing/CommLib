/* 
 * File:   Parser.h
 * Author: Administrator
 *
 * Created on 2014年1月27日, 下午4:12
 */

#ifndef PARSER_H
#define	PARSER_H
namespace CommLib {

    class Parser {
    public:
        Parser();
        Parser(const Parser& orig);
        virtual ~Parser();
    private:

    };

    class ProtocolParser : public Parser {
    public:
        virtual bool parse(char* buff, int len) = 0;
    };
}
#endif	/* PARSER_H */


#include "util.h"

const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if(!dot || dot == filename) return "";
	return dot + 1;
}

IplImage* skipNFrames(CvCapture* capture, int n)
{
	for(int i = 0; i < n; ++i)
	{
		if(cvQueryFrame(capture) == NULL)
		{
			return NULL;
		}
	}

	return cvQueryFrame(capture);
}

bool compareNat(const path& a, const path& b)
{
    if (a.empty())
        return true;
    if (b.empty())
        return false;
    if (isdigit(a.string()[0]) && !isdigit(b.string()[0]))
        return true;
    if (!isdigit(a.string()[0]) && isdigit(b.string()[0]))
        return false;
    if (!isdigit(a.string()[0]) && !isdigit(b.string()[0]))
    {
        if (a.string()[0] == b.string()[0])
            return compareNat(path(a.string().substr(1)), path(b.string().substr(1)));
        return (toupper(a.string()[0]) < toupper(b.string()[0]));
    }

    // Both strings begin with digit --> parse both numbers
    std::istringstream issa(a.string());
    std::istringstream issb(b.string());
    int ia, ib;
    issa >> ia;
    issb >> ib;
    if (ia != ib)
        return ia < ib;

    // Numbers are the same --> remove numbers and recurse
    std::string anew, bnew;
    std::getline(issa, anew);
    std::getline(issb, bnew);
    return (compareNat(anew, bnew));
}

// -1: s0 < s1; 0: s0 == s1; 1: s0 > s
bool numericCompare(const path &s0p, const path &s1p) {
	const string s0 = s0p.string();
	const string s1 = s1p.string();

    size_t i = 0, j = 0;
    for (; i < s0.size() && j < s1.size();) {
        string t0(1, s0[i++]);
        while (i < s0.size() && !(isdigit(t0[0]) ^ isdigit(s0[i]))) {
            t0.push_back(s0[i++]);
        }
        string t1(1, s1[j++]);
        while (j < s1.size() && !(isdigit(t1[0]) ^ isdigit(s1[j]))) {
            t1.push_back(s1[j++]);
        }
        if (isdigit(t0[0]) && isdigit(t1[0])) {
            size_t p0 = t0.find_first_not_of('0');
            size_t p1 = t1.find_first_not_of('0');
            t0 = p0 == string::npos ? "" : t0.substr(p0);
            t1 = p1 == string::npos ? "" : t1.substr(p1);
            if (t0.size() != t1.size()) {
                return t0.size() < t1.size() ? true : false;
            }
        }
        if (t0 != t1) {
            return t0 < t1 ? true : false;
        }
    }
    return i == s0.size() && j == s1.size() ? 0 : i != s0.size() ? false : true;
}

bool compare_filenames(path a, path b)
{
    char *pA, *pB;
    long A = strtol(a.string().c_str(), &pA, 10),
         B = strtol(b.string().c_str(), &pB, 10);
    if (A < B)
        return true;
    if (A == B)
        return strcmp(pA, pB);
    return false;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);   
    strftime(buf, sizeof(buf), "%X - %d/%m/%Y -> ", &tstruct);

    return buf;
}
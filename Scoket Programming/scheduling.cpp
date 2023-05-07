#include "scheduling.hpp"
#include "network.hpp"

using namespace std;


string remove_spaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}


string parse_input(string input) {
    vector<string> temp;
    istringstream iss(input);
    for(string s; iss >> s;) {
        temp.push_back(s);
    }

    // remove duplicates in the input
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    // convert to string format
    vector_to_str(temp, input, " ");

    return input;
}


int print_vector(vector<string> ret) {
    printf("[");
    for (int i = 0; i < ret.size(); ++i) {
        if (i == ret.size() - 1) {
            cout << ret[i] << "]" << endl;
        } else {
            cout << ret[i] << ", ";
        }
    }
    return 0;
}


int print_matrix(vector<vector<string>> test) {
    printf("[");
    for (int i = 0; i < test.size(); ++i) {
        for (int j = 0; j < test[i].size(); ++j) {
            if (j == 0) {
                cout << "[" << test[i][j] << ", ";
            } else if (j == test[i].size() - 1) {
                if (i == test.size() - 1) {
                    cout << test[i][j] << "]";
                } else {
                    cout << test[i][j] << "], ";
                }
            } else {
                cout << test[i][j] << ", ";
            }
        }
        if (i == test.size() - 1) {
            cout << "]" << endl;
        }
    }
    return 0;
}


int print_map(map<char, vector<string>> ret) {
    for (auto it = ret.begin(); it != ret.end(); ++it) {
        cout << it->first << ": ";
        vector<string> temp = it->second;
        for (int i = 0; i < temp.size(); ++i) {
            cout << temp[i] << " " << endl;
        }
    }
    return 0;
}


int common_time_to_str(vector<int> common_time, string &ret) {
    ret += "[";
    for (int i = 0; i < common_time.size(); i += 2) {
        if (i != 0) {
            ret += ", ";
        }
        string temp;
        string header = "[";
        string tail = "]";
        temp.append(header).append(to_string(common_time[i])).append(", ").append(to_string(common_time[i + 1])).append(
                tail);

        ret.append(temp);
    }
    ret += "]";
    return 0;
}


int combine_common_time(vector<string> common_time_str, string &ret) {
    vector<int> temp;
    vector<int> temp_final;
    stringstream ss;
    string token;

    for (int i = 0; i < common_time_str.size(); i++) {
        ss << common_time_str[i];
    }

    while (ss >> token) {
        stringstream st_str(token);
        while (st_str) {
            int i;
            if (st_str >> i) {
                temp.push_back(i);
            } else {
                st_str.clear();
                st_str.get();
            }
        }
    }

    for (int i = 0; i < temp.size() - 1; ++i) {
        if (temp[i] == temp[i + 1]) {
            ++i;
            continue;
        }
        temp_final.push_back(temp[i]);
    }
    temp_final.push_back(temp[temp.size() - 1]);
    common_time_to_str(temp_final, ret);
    return 0;
}


int vector_to_str(vector<string> name_vector, string &ret, string delim) {
    stringstream ss;
    for (auto it = name_vector.begin(); it != name_vector.end(); ++it) {
        if (it != name_vector.begin()) {
            ss << delim;
        }
        ss << *it;
    }
    ret = ss.str();
    return 0;
}


int msg_to_vector(string msg, vector<string> &msg_vector, char delim) {
    string token;
    stringstream ss(msg);

    while (getline(ss, token, delim)) {
        msg_vector.push_back(token);
    }
    return 0;
}


int find_name(string name, map<char, vector<string>> map_data, char &server_code) {
    for (auto it = map_data.begin(); it != map_data.end(); ++it) {
        vector<string> temp = it->second;
        for (int i = 0; i < temp.size(); ++i) {
            if (name == temp[i]) {
                server_code = it->first;
                return 1;
            }
        }
    }
    return 0;
}


int parse_time_slot(string time_slot_str, vector<string> &ret) {
    stringstream ss_str(time_slot_str);
    string token;
    vector<int> temp_int_vector;

    while (ss_str >> token) {
        stringstream st_str(token);
        while (st_str) {
            int i;
            if (st_str >> i) {
                temp_int_vector.push_back(i);
            } else {
                st_str.clear();
                st_str.get();
            }
        }
    }

    for (int i = 0; i < temp_int_vector.size(); i += 2) {
        int start_time = temp_int_vector[i];
        int end_time = temp_int_vector[i + 1];
        int count = 0;
        while (start_time + count < end_time) {
            string temp_str;
            string header = "[";
            string tail = "]";
            temp_str.append(header).append(to_string(start_time + count)).append(", ").append(
                    to_string(start_time + count + 1)).append(tail);
            ret.push_back(temp_str);
            count++;
        }
    }
    return 0;
}


void backend_server_command(int sockfd, string server_name, Scheduling &schedule) {
    string msg;
    int port;

    // Event: After receiving the usernames from the Main Server
    if (server_udp_recvfrom(sockfd, msg)) {
        return;
    }
    get_dynamic_port(sockfd, port);
    printf("Server %s received the usernames from Main Server using UDP over port %d.\n", server_name.c_str(), port);

    // Event: After running the algorithm to find the intersections of time
    // availability among all participants:
    vector<string> names_vector;
    vector<string> time_str_vector;
    vector<string> common_time_vector;

    msg_to_vector(msg, names_vector, ' ');

    vector<string> all_time_slot;
    for (int i = 0; i < 100; ++i) {
        string temp;
        string header = "[";
        string tail = "]";
        int j = i + 1;
        temp.append(header).append(to_string(i)).append(", ").append(to_string(j)).append(tail);
        all_time_slot.push_back(temp);
    }

    for (int i = 0; i < names_vector.size(); ++i) {
        time_str_vector.push_back(schedule.get_time(names_vector[i]));
    }

    vector<vector<string>> parsed_time_set;
    for (int i = 0; i < time_str_vector.size(); ++i) {
        vector<string> parsed_time_slot_vector;
        parse_time_slot(time_str_vector[i], parsed_time_slot_vector);
        parsed_time_set.push_back(parsed_time_slot_vector);
    }

    for (int i = 0; i < all_time_slot.size(); ++i) {
        bool free_flags = true;
        for (int j = 0; j < parsed_time_set.size(); ++j) {
            if (!(find(parsed_time_set[j].begin(), parsed_time_set[j].end(), all_time_slot[i]) !=
                  parsed_time_set[j].end())) {
                free_flags = false;
                break;
            }
        }
        if (free_flags) {
            common_time_vector.push_back(all_time_slot[i]);
        }
    }

    string common_time_msg;
    string names_print_out;
    if (common_time_vector.empty()) {
        common_time_msg = "[]";
    } else {
        combine_common_time(common_time_vector, common_time_msg);
    }

    vector_to_str(names_vector, names_print_out, ", ");

    printf("Found the intersection result: %s for %s.\n", common_time_msg.c_str(), names_print_out.c_str());

    {
        stringstream ss;
        ss << common_time_msg;

        // set a timer for backend server B
        if (server_name == "B") {
            unsigned int microseconds = 10000;
            usleep(microseconds);
        }

        if (client_udp_sendto(sockfd, "localhost", PORT_SERVER_M_UDP, ss.str())) {
            return;
        }
    }
    printf("Server %s finished sending the response to Main Server.\n", server_name.c_str());

}


void main_server_command(int sockfd_udp, int sockfd_tcp, int child_sockfd, map<char, vector<string>> map_data) {
    string msg;
    int num_backend_server = 2;
    vector<string> ports = {PORT_SERVER_A_UDP, PORT_SERVER_B_UDP};

    // Event: After receiving the request from client

    if (main_server_tcp_recv(child_sockfd, msg)) {
        return;
    }
    printf("Main Server received the request from client using TCP over port %s.\n", PORT_SERVER_M_TCP);

    vector<string> temp;
    msg_to_vector(msg, temp, ' ');

    char server_code;
    vector<string> name_list_a;
    vector<string> name_list_b;
    vector<string> not_found_name_list;

    for (int i = 0; i < temp.size(); ++i) {
        int flags = find_name(temp[i], map_data, server_code);
        if (flags) {
            if (server_code == 'A') {
                name_list_a.push_back(temp[i]);
            } else {
                name_list_b.push_back(temp[i]);
            }
        } else {
            not_found_name_list.push_back(temp[i]);
        }
    }
    vector<vector<string>> name_lists = {name_list_a, name_list_b};

    // send msg back to client if name not found
    if (!not_found_name_list.empty()) {
        string no_name_str;
        string no_name_print_out;

        // getting the printout message
        vector_to_str(not_found_name_list, no_name_print_out, ", ");

        // getting the send message
        vector_to_str(not_found_name_list, no_name_str, " ");
        no_name_str = "0;" + no_name_str;

        if (main_server_tcp_send(child_sockfd, no_name_str)) {
            return;
        }

        printf("%s do not exist. Send a reply to the client.\n", no_name_print_out.c_str());
    }

    // loop through backend servers and send sub name list
    for (int i = 0; i < num_backend_server; ++i) {
        string send_name_msg;
        string send_name_print_out;

        if (!name_lists[i].empty()) {
            vector_to_str(name_lists[i], send_name_print_out, ", ");
            vector_to_str(name_lists[i], send_name_msg, " ");
            printf("Found %s located at Server %c. Send to Server %c.\n", send_name_print_out.c_str(), 'A' + i,
                   'A' + i);

            if (client_udp_sendto(sockfd_udp, "localhost", ports[i], send_name_msg)) {
                return;
            }
        }
    }

    // loop through backend server and recv
    vector<string> rec_time_list;
    for (int i = 0; i < num_backend_server; ++i) {
        if (!name_lists[i].empty()) {
            string rec_msg;
            if (server_udp_recvfrom(sockfd_udp, rec_msg)) {
                return;
            }
            rec_time_list.push_back(rec_msg);

            int port;
            get_dynamic_port(sockfd_udp, port);
            printf("Main Server received from server %c the intersection result using UDP over port: %d.\n", 'A' + i,
                   port);
            printf("%s.\n", rec_msg.c_str());
        }
    }

    // find the intersection between results from server A and server B
    vector<string> common_time_vector;
    string common_time_msg_print_out;

    if (name_list_a.empty() && name_list_b.empty()) {
        common_time_msg_print_out = "[]";
    } else {
        vector<string> all_time_slot;
        for (int i = 0; i < 100; ++i) {
            string temp_str;
            string header = "[";
            string tail = "]";
            int j = i + 1;
            temp_str.append(header).append(to_string(i)).append(", ").append(to_string(j)).append(tail);
            all_time_slot.push_back(temp_str);
        }

        vector<vector<string>> parsed_time_set;
        for (int i = 0; i < rec_time_list.size(); ++i) {
            vector<string> parsed_time_slot_vector;
            parse_time_slot(rec_time_list[i], parsed_time_slot_vector);
            parsed_time_set.push_back(parsed_time_slot_vector);
        }

        for (int i = 0; i < all_time_slot.size(); ++i) {
            bool free_flags = true;
            for (int j = 0; j < parsed_time_set.size(); ++j) {
                if (!(find(parsed_time_set[j].begin(), parsed_time_set[j].end(), all_time_slot[i]) !=
                      parsed_time_set[j].end())) {
                    free_flags = false;
                    break;
                }
            }
            if (free_flags) {
                common_time_vector.push_back(all_time_slot[i]);
            }
        }

        if (common_time_vector.empty()) {
            common_time_msg_print_out = "[]";
        } else {
            combine_common_time(common_time_vector, common_time_msg_print_out);
        }
    }

    printf("Found the intersection between the results from server A and B:\n%s.\n", common_time_msg_print_out.c_str());

    for (int i = 0; i < not_found_name_list.size(); ++i) {
        auto it = find(temp.begin(), temp.end(), not_found_name_list[i]);
        if (it != temp.end()) {
            temp.erase(it);
        }
    }

    // set timer
    unsigned int microseconds = 200000;
    usleep(microseconds);

    if (!temp.empty()) {
        string to_client_name_list;
        vector_to_str(temp, to_client_name_list, ", ");

        string msg_send = "1;" + common_time_msg_print_out + ";" + to_client_name_list;

        if (main_server_tcp_send(child_sockfd, msg_send)) {
            return;
        }
    } else {
        string msg_send = "1;" + common_time_msg_print_out;

        if (main_server_tcp_send(child_sockfd, msg_send)) {
            return;
        }
    }

    printf("Main Server sent the result to the client.\n");

    close(child_sockfd);
}


void client_command(int sockfd) {
    string str;
    string msg;
    vector<string> recv_msg_vector;

    printf("Please enter the usernames to check schedule availability:\n");
    if (getline(cin, str)) {
        str = parse_input(str);
        if (str.empty()) {
            return;
        }

        if (main_server_tcp_send(sockfd, str)) {
            return;
        }
        printf("Client finished sending the usernames to Main Server.\n");

        if (main_server_tcp_recv(sockfd, msg)) {
            return;
        }

        msg_to_vector(msg, recv_msg_vector, ';');

        if (recv_msg_vector[0] == "0") {
            string sec_msg;
            int port;
            get_dynamic_port(sockfd, port);
            printf("Client received the reply from the Main Server using TCP over port: %d.\n", port);

            vector<string> not_found_name_list;
            recv_msg_vector.erase(recv_msg_vector.begin());
            msg_to_vector(recv_msg_vector[0], not_found_name_list, ' ');

            string not_found_name_print_out;
            vector_to_str(not_found_name_list, not_found_name_print_out, ", ");

            printf("%s do not exist.\n", not_found_name_print_out.c_str());

            if (main_server_tcp_recv(sockfd, sec_msg)) {
                return;
            }

            vector<string> final_schedule_list;
            msg_to_vector(sec_msg, final_schedule_list, ';');
            final_schedule_list.erase(final_schedule_list.begin());

            get_dynamic_port(sockfd, port);

            if (final_schedule_list[0] == "[]") {
                if (final_schedule_list.size() == 1) {
                    printf("Client received the reply from the Main Server using TCP over port %d:\nTime intervals %s works for %s.\n",
                           port, final_schedule_list[0].c_str(), "");
                } else {
                    printf("Client received the reply from the Main Server using TCP over port %d:\nTime intervals %s works for %s.\n",
                           port, final_schedule_list[0].c_str(), final_schedule_list[1].c_str());
                }
            } else {
                printf("Client received the reply from the Main Server using TCP over port %d:\nTime intervals %s works for %s.\n",
                       port, final_schedule_list[0].c_str(), final_schedule_list[1].c_str());
            }

        } else {
            vector<string> final_schedule_list;
            msg_to_vector(msg, final_schedule_list, ';');
            final_schedule_list.erase(final_schedule_list.begin());

            int port;
            get_dynamic_port(sockfd, port);

            if (final_schedule_list[0] == "[]") {
                if (final_schedule_list.size() == 1) {
                    printf("Client received the reply from the Main Server using TCP over port %d:\nTime intervals %s works for %s.\n",
                           port, final_schedule_list[0].c_str(), "");
                } else {
                    printf("Client received the reply from the Main Server using TCP over port %d:\nTime intervals %s works for %s.\n",
                           port, final_schedule_list[0].c_str(), final_schedule_list[1].c_str());
                }
            } else {
                printf("Client received the reply from the Main Server using TCP over port %d:\nTime intervals %s works for %s.\n",
                       port, final_schedule_list[0].c_str(), final_schedule_list[1].c_str());
            }
        }

        printf("-----Start a new request-----\n");
    }
}


Scheduling::Scheduling(string filename) {
    this->schedule_map = this->build_map(filename);
}


map<string, string> Scheduling::build_map(string filename) {
    map<string, string> schedule_map;
    ifstream txt_file;
    txt_file.open(filename);

    if (!txt_file) {
        fprintf(stderr, "ERROR: cannot open file %s.\n", filename.c_str());
        exit(-1);
    }

    while (txt_file) {
        string key{}, value{};
        {
            getline(txt_file, key, ';') && getline(txt_file, value);
            if (txt_file) {
                key = remove_spaces(key);
                value = remove_spaces(value);
                schedule_map[key] = value;
            }
        }
    }
    return schedule_map;
}


string Scheduling::get_time(string name) {
    string ret;
    map<string, string>::iterator i;
    i = schedule_map.find(name);
    if (i != schedule_map.end()) {
        ret = i->second;
    }
    return ret;
}


string Scheduling::get_name() {
    vector<string> temp;
    string ret;
    for (auto it = schedule_map.begin(); it != schedule_map.end(); ++it) {
        temp.push_back(it->first);
    }
    for (int i = 0; i < temp.size(); ++i) {
        if (i == temp.size() - 1) {
            ret += temp[i];
        } else {
            ret += temp[i] + " ";
        }
    }
    return ret;
}
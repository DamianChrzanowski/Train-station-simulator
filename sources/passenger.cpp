#include "passenger.hpp"

#include <string>
#include "stage.hpp"

passenger::passenger(int id, target main_target, bool top, bool tired, bool ticket, transfer_center& tc, std::atomic<bool>& end) :
            id(id), main_target(main_target), top(top), tired(tired), ticket(ticket), tc(tc), end(end), passenger_thread(&passenger::lifeline, this) {}

void passenger::lifeline() 
{
    std::string id_as_string = std::to_string(id);
    std::string info;
    screen_printer& printer = tc.get_screen_printer();
    

    if (tired) {
        if (top) {
            for (int i {0}; i < 8; ++i) {
                bench& b = tc.get_top_benches().at(i);

                if (b.get_mutex().try_lock()) {
                    if (b.is_free()){
                        printer.print_group(b.get_x(), b.get_y(), id);
                        
                        b.request(id);

                        info = "passenger(" + id_as_string  + ") sits";
                        printer.print_info(&info[0]);

                        std::uniform_int_distribution<> dis(55, 95);
                        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

                        printer.clear_group(b.get_x(), b.get_y());
                        b.release();
                        b.get_mutex().unlock();

                        break;
                    }
                }

                b.get_mutex().unlock();
            }
        } else {
            for (int i {0}; i < 8; ++i) {
                bench& b = tc.get_down_benches().at(i);

                if (b.get_mutex().try_lock()) {
                    if (b.is_free()){
                        printer.print_group(b.get_x(), b.get_y(), id);
                        
                        b.request(id);
                        info = "passenger(" + id_as_string  + ") sits";
                        printer.print_info(&info[0]);

                        std::uniform_int_distribution<> dis(55, 95);
                        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng) * 100));

                        printer.clear_group(b.get_x(), b.get_y());
                        b.release();
                        b.get_mutex().unlock();

                        break;
                    }
                }

                b.get_mutex().unlock();
            }
        }
    }

    if (main_target == target::leave_transfer_center) {
        info = "passenger(" + id_as_string  + ") wants leave transfer center";

        if (!top) {
            std::uniform_int_distribution<> dis(0, 1);
            int stairs{ dis(rng) }; 
            bool took_stairs { false };

            while (!took_stairs) {
                stairs = (stairs + 1) % 2;
                std::mutex& stairs_mutex = tc.get_tc_stairs().at(stairs).get_mutex();

                if (stairs_mutex.try_lock()) {
                    if (tc.get_tc_stairs().at(stairs).is_free()) {
                        auto& st = tc.get_tc_stairs().at(stairs);
                        st.request(id);
                        printer.print_group(st.get_x_down(), st.get_y_down(), id);
                        info = "passenger(" + id_as_string  + ") goes up stairs";
                        printer.print_info(&info[0]);
                        took_stairs = true;
                    }
                    stairs_mutex.unlock(); 
                }
            }

            auto& st = tc.get_tc_stairs().at(stairs);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            printer.clear_group(st.get_x_down(), st.get_y_down());
            
            printer.print_group(st.get_x_top(), st.get_y_top(), id);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            st.release();
            printer.clear_group(st.get_x_top(), st.get_y_top());

            info = "passenger(" + id_as_string  + ") climbed stairs";
            printer.print_info(&info[0]);
            top = true;
        }

        info = "passenger(" + id_as_string  + ") leaves transfer center";
        printer.print_info(&info[0]);

    } else {
        if (!ticket && !top) {
            std::uniform_int_distribution<> dis(0, 1);
            int stairs{ dis(rng) }; 
            bool took_stairs { false };

            while (!took_stairs) {
                stairs = (stairs + 1) % 2;
                std::mutex& stairs_mutex = tc.get_tc_stairs().at(stairs).get_mutex();

                if (stairs_mutex.try_lock()) {
                    if (tc.get_tc_stairs().at(stairs).is_free()) {
                        auto& st = tc.get_tc_stairs().at(stairs);
                        st.request(id);
                        printer.print_group(st.get_x_down(), st.get_y_down(), id);
                        info = "passenger(" + id_as_string  + ") goes up stairs";
                        printer.print_info(&info[0]);
                        took_stairs = true;
                    }
                    stairs_mutex.unlock(); 
                }
            }

            auto& st = tc.get_tc_stairs().at(stairs);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            printer.clear_group(st.get_x_down(), st.get_y_down());
            
            printer.print_group(st.get_x_top(), st.get_y_top(), id);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            st.release();
            printer.clear_group(st.get_x_top(), st.get_y_top());

            info = "passenger(" + id_as_string  + ") climbed stairs";
            printer.print_info(&info[0]);
            top = true;
        }
        
        if (!ticket) {
            info = "passenger(" + id_as_string  + ") goes to get ticket";
            printer.print_info(&info[0]);
            std::uniform_int_distribution<> dis(0, 1);
            int line{ dis(rng) };
            bool took_line { false };
            int number_in_line{ 4 };

            // out of line
            while (!took_line) {
                line = (line + 1) % 2;
                std::mutex& place_in_line = tc.get_lines_tm().at(line).at(number_in_line).get_mutex();

                if (place_in_line.try_lock()) {
                    if (tc.get_lines_tm().at(line).at(number_in_line).is_free()) {
                        auto& place = tc.get_lines_tm().at(line).at(number_in_line);
                        place.request(id);
                        printer.print_group(place.get_x(), place.get_y(), id);
                        info = "passenger(" + id_as_string  + ") entered line";
                        printer.print_info(&info[0]);
                        took_line = true;
                    }
                    place_in_line.unlock(); 
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            // when is in line
            while (number_in_line >= 1) {
                std::unique_lock<std::mutex> lock(tc.get_lines_tm().at(line).at(number_in_line - 1).get_mutex());
                auto& place = tc.get_lines_tm().at(line).at(number_in_line);
                auto& next_place = tc.get_lines_tm().at(line).at(number_in_line - 1);
                next_place.request(id);
                lock.unlock();
                place.release();
                info = "passenger(" + id_as_string  + ") moves in line";
                printer.print_info(&info[0]);
                printer.clear_group(place.get_x(), place.get_y());
                printer.print_group(next_place.get_x(), next_place.get_y(), id);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                --number_in_line;
            }

            // if passenger lefts line, he buys a ticket

            std::unique_lock<std::mutex> lock(tc.get_ticket_machines().at(line).get_mutex());
            tc.get_ticket_machines().at(line).request(id);
            lock.unlock();

            tc.get_lines_tm().at(line).at(0).release();
            printer.clear_group(tc.get_lines_tm().at(line).at(0).get_x(), tc.get_lines_tm().at(line).at(0).get_y());

            printer.print_group(tc.get_ticket_machines().at(line).get_x(), tc.get_ticket_machines().at(line).get_y(), id);
            info = "passenger(" + id_as_string  + ") buys ticket";
            printer.print_info(&info[0]);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5500));
            
            tc.get_ticket_machines().at(line).release();
            printer.clear_group(tc.get_ticket_machines().at(line).get_x(), tc.get_ticket_machines().at(line).get_y());
            info = "passenger(" + id_as_string  + ") bought ticket";
            printer.print_info(&info[0]);
        }

        if (main_target == target::subway_east || main_target == target::subway_west) {
            if (top) {
                std::uniform_int_distribution<> dis(0, 1);
                int stairs{ dis(rng) }; 
                bool took_stairs { false };

                while (!took_stairs) {
                    stairs = (stairs + 1) % 2;
                    std::mutex& stairs_mutex = tc.get_tc_stairs().at(stairs).get_mutex();

                    if (stairs_mutex.try_lock()) {
                        if (tc.get_tc_stairs().at(stairs).is_free()) {
                            auto& st = tc.get_tc_stairs().at(stairs);
                            st.request(id);
                            printer.print_group(st.get_x_top(), st.get_y_top(), id);
                            info = "passenger(" + id_as_string  + ") goes down stairs";
                            printer.print_info(&info[0]);
                            took_stairs = true;
                        }
                        stairs_mutex.unlock(); 
                    }
                }

                auto& st = tc.get_tc_stairs().at(stairs);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                printer.clear_group(st.get_x_top(), st.get_y_top());

                printer.print_group(st.get_x_down(), st.get_y_down(), id);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                st.release();
                printer.clear_group(st.get_x_down(), st.get_y_down());

                info = "passenger(" + id_as_string  + ") went stairs";
                printer.print_info(&info[0]);
                top = false;
            }

             if (main_target == target::subway_west) {
                bool took_transport {false};

                // printer.print_info("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

                while (!took_transport) {
                    // printer.print_info("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
                    tc.subway_west_channel.wait();
                    // printer.print_info("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
                    int door{ 0 }; 
                    bool took_door { false };

                    while (!took_door) {
                        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        door = (door + 1) % 2;

                        auto& d = tc.get_west_subway().get()->get_doors().at(door);


                        if (d.get_mutex().try_lock()) {
                            if (d.are_open()) {
                                
                                d.request(id);
                                info = "passenger(" + id_as_string  + ") entered transport";
                                printer.print_info(&info[0]);

                                if (door == 0)
                                    printer.print_group(144, 6, id);
                                else
                                    printer.print_group(150, 6, id);

                                took_door = true;
                                took_transport = true;

                                std::this_thread::sleep_for(std::chrono::milliseconds(220));

                                if (door == 0)
                                    printer.clear_group(144, 6);
                                else
                                    printer.clear_group(150, 6);

                                
                            }

                            d.get_mutex().unlock(); 
                        }
                        d.release();

                        if (tc.get_west_subway().get()->get_stage() == stage::closing_doors || tc.get_west_subway().get()->get_stage() == stage::departure)
                            break;
                    }

                    // printer.print_info("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");               
                }

                printer.print_info("EEEEEEEEEEEEEEEEEEEEEEEEEEEESUBWAYWEST");
            } else {
                bool took_transport {false};

                // printer.print_info("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

                while (!took_transport) {
                    // printer.print_info("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
                    tc.subway_east_channel.wait();
                    // printer.print_info("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
                    int door{ 0 }; 
                    bool took_door { false };

                    while (!took_door) {
                        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        door = (door + 1) % 2;

                        auto& d = tc.get_east_subway().get()->get_doors().at(door);

                        if (d.get_mutex().try_lock()) {
                            if (d.are_open()) {
                                
                                d.request(id);
                                info = "passenger(" + id_as_string  + ") entered transport";
                                printer.print_info(&info[0]);

                                if (door == 0)
                                    printer.print_group(144, 55, id);
                                else
                                    printer.print_group(150, 55, id);

                                took_door = true;
                                took_transport = true;

                                std::this_thread::sleep_for(std::chrono::milliseconds(220));

                                if (door == 0)
                                    printer.clear_group(144, 55);
                                else
                                    printer.clear_group(150, 55);

                            }

                            d.get_mutex().unlock(); 
                        }

                        d.release();

                        if (tc.get_east_subway().get()->get_stage() == stage::closing_doors || tc.get_east_subway().get()->get_stage() == stage::departure)
                            break;
                    }

                    printer.print_info("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEESUBWAYEAST");               
                }
            }

        }

        if (main_target == target::bus_east || main_target == target::bus_west) {
            if (!top) {
                std::uniform_int_distribution<> dis(0, 1);
                int stairs{ dis(rng) }; 
                bool took_stairs { false };

                while (!took_stairs) {
                    stairs = (stairs + 1) % 2;
                    std::mutex& stairs_mutex = tc.get_tc_stairs().at(stairs).get_mutex();

                    if (stairs_mutex.try_lock()) {
                        if (tc.get_tc_stairs().at(stairs).is_free()) {
                            auto& st = tc.get_tc_stairs().at(stairs);
                            st.request(id);
                            printer.print_group(st.get_x_down(), st.get_y_down(), id);
                            info = "passenger(" + id_as_string  + ") goes up stairs";
                            printer.print_info(&info[0]);
                            took_stairs = true;
                        }
                        stairs_mutex.unlock(); 
                    }
                }

                auto& st = tc.get_tc_stairs().at(stairs);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                printer.clear_group(st.get_x_down(), st.get_y_down());
                
                printer.print_group(st.get_x_top(), st.get_y_top(), id);
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                st.release();
                printer.clear_group(st.get_x_top(), st.get_y_top());

                info = "passenger(" + id_as_string  + ") climbed stairs";
                printer.print_info(&info[0]);
                top = true;
            }

            if (main_target == target::bus_west) {
                bool took_transport {false};

                // printer.print_info("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

                while (!took_transport) {
                    // printer.print_info("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
                    tc.bus_west_channel.wait();
                    // printer.print_info("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
                    int door{ 0 }; 
                    bool took_door { false };

                    while (!took_door) {
                        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        door = (door + 1) % 2;

                        auto& d = tc.get_west_bus().get()->get_doors().at(door);

                        if (d.get_mutex().try_lock()) {
                            if (d.are_open()) {
                                
                                d.request(id);
                                info = "passenger(" + id_as_string  + ") entered transport";
                                printer.print_info(&info[0]);

                                if (door == 0)
                                    printer.print_group(44, 6, id);
                                else
                                    printer.print_group(50, 6, id);

                                took_door = true;
                                took_transport = true;

                                std::this_thread::sleep_for(std::chrono::milliseconds(220));

                                if (door == 0)
                                    printer.clear_group(44, 6);
                                else
                                    printer.clear_group(50, 6);
                            }

                            d.get_mutex().unlock(); 
                        }

                        d.release();

                        if (tc.get_west_bus().get()->get_stage() == stage::closing_doors || tc.get_west_bus().get()->get_stage() == stage::departure)
                            break;
                    }

                    // printer.print_info("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");               
                }

                printer.print_info("EEEEEEEEEEEEEEEEEEEEEEEEEEEBUSWEST");
            } else {
                bool took_transport {false};

                // printer.print_info("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

                while (!took_transport) {
                    // printer.print_info("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
                    tc.bus_east_channel.wait();
                    // printer.print_info("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
                    int door{ 0 }; 
                    bool took_door { false };

                    while (!took_door) {
                        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        door = (door + 1) % 2;

                        auto& d = tc.get_east_bus().get()->get_doors().at(door);

                        if (d.get_mutex().try_lock()) {
                            if (d.are_open()) {
                                
                                d.request(id);
                                info = "passenger(" + id_as_string  + ") entered transport";
                                printer.print_info(&info[0]);
                                took_door = true;
                                took_transport = true;

                                if (door == 0)
                                    printer.print_group(44, 55, id);
                                else
                                    printer.print_group(50, 55, id);

                                std::this_thread::sleep_for(std::chrono::milliseconds(220));

                                if (door == 0)
                                    printer.clear_group(44, 55);
                                else
                                    printer.clear_group(50, 55);

                                
                            }

                            d.get_mutex().unlock(); 
                        }
                        d.release();

                        if (tc.get_east_bus().get()->get_stage() == stage::closing_doors || tc.get_east_bus().get()->get_stage() == stage::departure)
                            break;
                    }

                    printer.print_info("EEEEEEEEEEEEEEEEEEEEEEBUSEAST");               
                }

            }
            

        }
    }
}

int passenger::get_id()
{
    return id;
}

passenger::~passenger()
{
    passenger_thread.join();
}
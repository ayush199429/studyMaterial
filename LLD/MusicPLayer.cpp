/*
Design Amazon music search
User can search by songs, artists and album
User can create playlist
User can play, pause and reset the song
*/

class User{
    uint id;
    string name;
    string email;
};
class Song{
    uint id;
    string name;
    char* data;
    uint artistID;
    uint albumID;
    Date releaseDate;
};
class SongSrv{
    unordered_map<uint,Song> songsList;
    unordered_map<uint,vector<uint>> playLists; //playlistID vs songIDs
    void addSong(Song song){}
};
class PLayList{
    uint id;
    string name;
    vector<uint> songsList;
    uint usrID;
    Date createdOn;
};
class UserSrv{
    vector<User> usersList;
    unordered_map<uint,vector<uint>> likedSongs;
    unordered_map<uint,vector<uint>> playlists; //userID vs playListIDs
    void addToPlayList(uint userid, uint songid, uint playlistid){}
};
enum PLayerState{
    PLAYING,
    NOT_PLAYING
};
class MusicPlayer{
    Song currentSong;
    char* curSongBuffer;
    AudioPlayer player;
    uint currentTime;
    PlayerState state;
    void setPlayer(){}
    void play(uint songID){
        reset();
        dwld(songID, &currSongBuffer);
        player.play(currSongBuffer, currentTime);
    }
    void play(uint playListID){
        PlayList pl = UserSrv.getPlayList(playLIstID);
        for(uint id : playList)
            play(id);
    }
    void pause(){
        state = NOT_PLAYING;
        player.stop();
    }
    void resume{
        state = PLAYING;
        player.resume();
    }
    void stop(){
        reset = NOT_PLAYING;
        player.stop();
        currentTime = 0;
    }
};
class AmazonMusic{
    UserSrv usrsrv; //singleton
    SongSrv songsrv; //singleton
    MusicPlayer player;
    auto songid = SongSrv.find("ABC");
    player.play(songid);
};



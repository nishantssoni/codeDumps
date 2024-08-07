from instagram_private_api import Client, ClientCompatPatch
import getpass

def get_followers_and_following(username, password):
    """
    Fetches the list of followers and following users for the given Instagram account.
    
    Parameters:
    username (str): Instagram username.
    password (str): Instagram password.
    
    Returns:
    tuple: A tuple containing two lists:
        - followers: List of usernames who follow the user.
        - following: List of usernames whom the user is following.
    """
    try:
        # Create an instance of the Instagram API client with provided credentials
        api = Client(username, password)
        user_id = api.authenticated_user_id  # Get the ID of the authenticated user

        followers = []  # List to store followers' usernames
        following = []  # List to store following users' usernames
        rank_token = Client.generate_uuid()  # Generate a unique token for ranking

        # Fetch the list of followers and following users
        followers_response = api.user_followers(user_id, rank_token=rank_token)
        following_response = api.user_following(user_id, rank_token=rank_token)

        # Extract usernames of followers
        for follower in followers_response['users']:
            followers.append(follower['username'])

        # Extract usernames of following users
        for followee in following_response['users']:
            following.append(followee['username'])

        return followers, following  # Return the lists of followers and following

    except Exception as e:
        # Print an error message if there's an issue with username or password
        print('There is something wrong with the username or password')
        print(f"Error: {e}")
        getList()  # Prompt the user to enter credentials again

def getList():
    """
    Prompts the user for Instagram credentials, retrieves followers and following lists,
    and displays the list of usernames who do not follow back.
    """
    username = input("Enter your username: ")  # Prompt user for Instagram username
    password = getpass.getpass(prompt="Enter your password: ")  # Prompt user for Instagram password securely

    # Get followers and following lists
    your_followers, your_following = get_followers_and_following(username, password)

    # Print the number of followers and following users
    print('\n\n\nFollowers: ', len(your_followers), '  Following: ', len(your_following))
    print('\nHere is the list of usernames who do not follow you back:\n')

    # Print usernames of people who are followed but do not follow back
    for i in your_following:
        if i not in your_followers:
            print(i)

# Entry point of the script
if __name__ == "__main__":
    getList()  # Call the function to start the process

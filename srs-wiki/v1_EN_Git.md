[**HOME**](Home) > [**EN(1.0)**](v1_EN_Home) > [**Git**](v1_EN_Git)

# Git Usage

How to use stable version of SRS? How to update code?

## Checkout Branch

Some features are introduced in SRS2.0, the SRS1.0 does not support.
The wiki url specifies the version of SRS supports it.

To checkout SRS1.0 branch:

```
git pull && git checkout 1.0release
```

To checkout SRS2.0 branch:

```
git pull && git checkout 2.0release
```

To checkout SRS3.0 branch(if no 3.0release branch, it's develop):

```
git pull && git checkout develop
```

Note: The master branch is main release.

## SRS Branches

Each release will branch with hotfix, for example, 1.0release with latest bug fixed.

The main stable branch is the main release branch. The hotfixes will be merged from release to master every 1 or 2 months. For example, the hotfixes of 1.0release will be merged to master for 1.0r1, 1.0r2 to 1.0rN.

The develop is the dev branch, for example, 2.0 dev branch.

The stable is: master >= 1.0release >> develop.

## How to get code?

User can git clone the SRS project:

```bash
git clone https://github.com/simple-rtmp-server/srs
```

Mirrors: https://github.com/simple-rtmp-server/srs/tree/1.0release#mirrors

## How to update code?

Use git pull to update code, never git clone everytime.

```bash
git pull
```

Do not need to delete the objs, just update code and make. 
Use configure when make failed.

## How to sync with SRS project

Please read [Pro Git](http://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project)

## Fetch and Push

Please read [Pro Git](http://git-scm.com/book/en/v2/GitHub-Contributing-to-a-Project)

## Patch

You can format patch and email to `winlin at vip.126.com`, for example, patch SRS2:

```
git format-patch origin/2.0release
```

Winlin 2014.11
